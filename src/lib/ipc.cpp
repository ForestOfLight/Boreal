#include "lib/ipc.h"

#include <endstone/event/server/script_message_event.h>
#include <endstone/plugin/plugin.h>

std::vector<std::string> split(const std::string &s, const std::string &delim) {
    std::vector<std::string> elems;
    size_t start = 0;
    size_t end = s.find(delim);
    while (end != std::string::npos) {
        elems.push_back(s.substr(start, end - start));
        start = end + delim.length();
        end = s.find(delim, start);
    }
    elems.push_back(s.substr(start));
    return elems;
};

namespace proto::mips {
    std::string serialize(const std::deque<uint8_t> &stream) {
        constexpr char hex[] = "0123456789ABCDEF";
        std::string str = "(0x";
        for (const uint8_t v: stream) {
            str += hex[(v >> 4) & 0xF];
            str += hex[v & 0xF];
        }
        str += ")";
        return str;
    };
    std::deque<uint8_t> deserialize(const std::string &str) {
        std::deque<uint8_t> stream;
        if (str.substr(0, 3) == "(0x" && str.back() == ')') {
            const std::string hex_str = str.substr(3, str.size() - 4);
            for (size_t i = 0; i < hex_str.size(); i += 2) {
                std::string hex = hex_str.substr(i, 2);
                uint8_t byte = std::stoi(hex, nullptr, 16);
                stream.push_back(byte);
            }
        }
        return stream;
    };
} // namespace proto::mips

namespace net {
    std::vector<std::string> serialize(const std::deque<uint8_t> &stream,
                                       const size_t max_size = std::numeric_limits<size_t>::max()) {
        std::vector<std::string> result;

        std::string acc_str;
        size_t acc_size = 0;
        for (size_t i = 0; i < stream.size(); i++) {
            uint16_t char_code = stream[i];
            if (i + 1 < stream.size()) {
                char_code |= (stream[++i] << 8);
            }

            const size_t u16_size = char_code <= 0x7F ? 1 : char_code <= 0x7FF ? 2 : char_code <= 0xFFFF ? 3 : 4;
            const size_t char_size = char_code > 0xFF ? u16_size : 2;

            if (acc_size + char_size > max_size) {
                result.push_back(acc_str);
                acc_str.clear();
                acc_size = 0;
            }

            if (char_code > 0xFF) {
                if (char_code <= 0x7F) {
                    acc_str += static_cast<char>(char_code);
                } else if (char_code <= 0x7FF) {
                    acc_str += static_cast<char>(0xC0 | (char_code >> 6));
                    acc_str += static_cast<char>(0x80 | (char_code & 0x3F));
                } else {
                    acc_str += static_cast<char>(0xE0 | (char_code >> 12));
                    acc_str += static_cast<char>(0x80 | (char_code >> 6) & 0x3F);
                    acc_str += static_cast<char>(0x80 | (char_code & 0x3F));
                }
                acc_size += char_size;
            } else {
                constexpr char hex[] = "0123456789ABCDEF";
                acc_str += hex[(char_code >> 4) & 0xf];
                acc_str += hex[char_code & 0xf];
                acc_size += 2;
            }
        }
        result.push_back(acc_str);

        return result;
    }

    std::deque<uint8_t> deserialize(const std::vector<std::string> &strings) {
        std::deque<uint8_t> result;
        for (const std::string &str: strings) {
            for (size_t j = 0; j < str.size(); j++) {
                uint16_t char_code = 0;

                if (const uint8_t ch = str[j]; ch <= 0x7F) {
                    char_code = ch;
                } else if (ch <= 0xDF && j + 1 < str.length()) {
                    char_code |= (ch & 0x1F) << 6;
                    char_code |= (str[++j] & 0x3F);
                } else if (ch <= 0xEF && j + 2 < str.length()) {
                    char_code |= (ch & 0x0F) << 12;
                    char_code |= (str[++j] & 0x3F) << 6;
                    char_code |= (str[++j] & 0x3F);
                }

                if (char_code <= 0xFF) {
                    std::string hex;
                    hex += str[j];
                    if (j + 1 < str.length()) {
                        hex += str[++j];
                    } else
                        hex += "0";
                    const uint16_t hex_code = std::stoi(hex, nullptr, 16);
                    result.push_back(hex_code & 0xFF);
                    result.push_back(hex_code >> 8);
                } else {
                    result.push_back(char_code & 0xFF);
                    result.push_back(char_code >> 8);
                }
            }
        }
        return result;
    }

    void on_script_message_event(const endstone::ScriptMessageEvent &event, const endstone::Plugin &plugin) {
        const std::vector<std::string> split_id = split(event.getMessageId(), ":");

        std::deque<uint8_t> endpoint_stream = proto::mips::deserialize(split_id[0]);
        const std::string endpoint = proto::endpoint::deserialize(endpoint_stream);

        const auto &listeners = endpoints[endpoint];
        if (event.getSender().asConsole() != nullptr) {
            std::deque<uint8_t> header_stream = proto::mips::deserialize(split_id[1]);
            const proto::header header = proto::header::deserialize(header_stream);

            for (const auto &listener: listeners) {
                if (listener) {
                    (*listener)(header, event.getMessage());
                }
            }

            if (scriptevent_debugging) {
                const auto &data = deserialize(std::vector{event.getMessage()});
                std::string data_str{};
                for (const uint8_t &byte: data) {
                    constexpr char hex[] = "0123456789ABCDEF";
                    data_str += hex[(byte >> 4) & 0xF];
                    data_str += hex[byte & 0xF];
                    data_str += ",";
                }
                data_str.pop_back();

                plugin.getLogger().info(
                        R"((MCBE-IPC) Endpoint: "{}", Header: {{ guid: "{}", encoding: "{}", index: {}, final: {} }}, Data: [{}])",
                        endpoint, header.guid, header.encoding, header.index, header.final, data_str);
            }
        }
    }
} // namespace net