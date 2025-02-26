#pragma once

#include <chrono>
#include <deque>
#include <endstone/command/console_command_sender.h>
#include <endstone/plugin/plugin.h>
#include <functional>
#include <future>
#include <memory>
#include <random>
#include <string>
#include <vector>
#include <endstone/event/server/script_message_event.h>

namespace endstone {
    class Plugin;
    class ScriptMessageEvent;
} // namespace endstone

namespace proto {
    template<typename T>
    struct serializable {
        using value_type = T;
        virtual ~serializable() = default;
        static void serialize(const T &, std::deque<uint8_t> &) {
            static_assert(sizeof(T) == -1, "Serialize method not implemented for this type");
        }
        static T deserialize(std::deque<uint8_t> &) {
            static_assert(sizeof(T) == -1, "Deserialize method not implemented for this type");
            return T{};
        }
    };

    namespace mips {
        std::string serialize(const std::deque<uint8_t> &stream);
        std::deque<uint8_t> deserialize(const std::string &str);
    } // namespace mips

    struct null final : serializable<nullptr_t> {
        static void serialize(const std::nullptr_t &, std::deque<uint8_t> &) {}
        static std::nullptr_t deserialize(std::deque<uint8_t> &) { return nullptr; }
    };
    struct int8 final : serializable<int8_t> {
        static void serialize(const value_type &value, std::deque<uint8_t> &stream) {
            uint8_t bytes[sizeof(value_type)];
            std::memcpy(bytes, &value, sizeof(value_type));
            std::reverse(bytes, bytes + sizeof(value_type));
            for (unsigned char byte: bytes) {
                stream.push_back(byte);
            }
        }

        static value_type deserialize(std::deque<uint8_t> &stream) {
            value_type value;
            uint8_t bytes[sizeof(value_type)];
            for (unsigned char &byte: bytes) {
                byte = stream.front();
                stream.pop_front();
            }
            std::reverse(bytes, bytes + sizeof(value_type));
            std::memcpy(&value, bytes, sizeof(value_type));
            return value;
        }
    };
    struct int16 final : serializable<int16_t> {
        static void serialize(const value_type &value, std::deque<uint8_t> &stream) {
            uint8_t bytes[sizeof(value_type)];
            std::memcpy(bytes, &value, sizeof(value_type));
            std::reverse(bytes, bytes + sizeof(value_type));
            for (unsigned char byte: bytes) {
                stream.push_back(byte);
            }
        }

        static value_type deserialize(std::deque<uint8_t> &stream) {
            value_type value;
            uint8_t bytes[sizeof(value_type)];
            for (unsigned char &byte: bytes) {
                byte = stream.front();
                stream.pop_front();
            }
            std::reverse(bytes, bytes + sizeof(value_type));
            std::memcpy(&value, bytes, sizeof(value_type));
            return value;
        }
    };
    struct int32 final : serializable<int32_t> {
        static void serialize(const value_type &value, std::deque<uint8_t> &stream) {
            uint8_t bytes[sizeof(value_type)];
            std::memcpy(bytes, &value, sizeof(value_type));
            std::reverse(bytes, bytes + sizeof(value_type));
            for (unsigned char byte: bytes) {
                stream.push_back(byte);
            }
        }

        static value_type deserialize(std::deque<uint8_t> &stream) {
            value_type value;
            uint8_t bytes[sizeof(value_type)];
            for (unsigned char &byte: bytes) {
                byte = stream.front();
                stream.pop_front();
            }
            std::reverse(bytes, bytes + sizeof(value_type));
            std::memcpy(&value, bytes, sizeof(value_type));
            return value;
        }
    };
    struct uint8 final : serializable<uint8_t> {
        static void serialize(const value_type &value, std::deque<uint8_t> &stream) {
            uint8_t bytes[sizeof(value_type)];
            std::memcpy(bytes, &value, sizeof(value_type));
            std::reverse(bytes, bytes + sizeof(value_type));
            for (unsigned char byte: bytes) {
                stream.push_back(byte);
            }
        }

        static value_type deserialize(std::deque<uint8_t> &stream) {
            value_type value;
            uint8_t bytes[sizeof(value_type)];
            for (unsigned char &byte: bytes) {
                byte = stream.front();
                stream.pop_front();
            }
            std::reverse(bytes, bytes + sizeof(value_type));
            std::memcpy(&value, bytes, sizeof(value_type));
            return value;
        }
    };
    struct uint16 final : serializable<uint16_t> {
        static void serialize(const value_type &value, std::deque<uint8_t> &stream) {
            uint8_t bytes[sizeof(value_type)];
            std::memcpy(bytes, &value, sizeof(value_type));
            std::reverse(bytes, bytes + sizeof(value_type));
            for (unsigned char byte: bytes) {
                stream.push_back(byte);
            }
        }

        static value_type deserialize(std::deque<uint8_t> &stream) {
            value_type value;
            uint8_t bytes[sizeof(value_type)];
            for (unsigned char &byte: bytes) {
                byte = stream.front();
                stream.pop_front();
            }
            std::reverse(bytes, bytes + sizeof(value_type));
            std::memcpy(&value, bytes, sizeof(value_type));
            return value;
        }
    };
    struct uint32 final : serializable<uint32_t> {
        static void serialize(const value_type &value, std::deque<uint8_t> &stream) {
            uint8_t bytes[sizeof(value_type)];
            std::memcpy(bytes, &value, sizeof(value_type));
            std::reverse(bytes, bytes + sizeof(value_type));
            for (unsigned char byte: bytes) {
                stream.push_back(byte);
            }
        }

        static value_type deserialize(std::deque<uint8_t> &stream) {
            value_type value;
            uint8_t bytes[sizeof(value_type)];
            for (unsigned char &byte: bytes) {
                byte = stream.front();
                stream.pop_front();
            }
            std::reverse(bytes, bytes + sizeof(value_type));
            std::memcpy(&value, bytes, sizeof(value_type));
            return value;
        }
    };
    struct uvarint32 final : serializable<uint32_t> {
        static void serialize(const uint32_t &value, std::deque<uint8_t> &stream) {
            uint32_t temp = value;
            while (temp >= 0x80) {
                stream.push_back((temp & 0x7f) | 0x80);
                temp >>= 7;
            }
            stream.push_back(temp);
        }

        static uint32_t deserialize(std::deque<uint8_t> &stream) {
            uint32_t value = 0;
            size_t size = 0;
            uint8_t byte;
            do {
                byte = stream.front();
                stream.pop_front();
                value |= (byte & 0x7f) << (size * 7);
                size += 1;
            } while ((byte & 0x80) != 0 && size < 10);
            return value;
        }
    };
    struct float32 final : serializable<float_t> {
        static void serialize(const value_type &value, std::deque<uint8_t> &stream) {
            uint8_t bytes[sizeof(value_type)];
            std::memcpy(bytes, &value, sizeof(value_type));
            std::reverse(bytes, bytes + sizeof(value_type));
            for (unsigned char byte: bytes) {
                stream.push_back(byte);
            }
        }

        static value_type deserialize(std::deque<uint8_t> &stream) {
            value_type value;
            uint8_t bytes[sizeof(value_type)];
            for (unsigned char &byte: bytes) {
                byte = stream.front();
                stream.pop_front();
            }
            std::reverse(bytes, bytes + sizeof(value_type));
            std::memcpy(&value, bytes, sizeof(value_type));
            return value;
        }
    };
    struct float64 final : serializable<double_t> {
        static void serialize(const value_type &value, std::deque<uint8_t> &stream) {
            uint8_t bytes[sizeof(value_type)];
            std::memcpy(bytes, &value, sizeof(value_type));
            std::reverse(bytes, bytes + sizeof(value_type));
            for (unsigned char byte: bytes) {
                stream.push_back(byte);
            }
        }

        static value_type deserialize(std::deque<uint8_t> &stream) {
            value_type value;
            uint8_t bytes[sizeof(value_type)];
            for (unsigned char &byte: bytes) {
                byte = stream.front();
                stream.pop_front();
            }
            std::reverse(bytes, bytes + sizeof(value_type));
            std::memcpy(&value, bytes, sizeof(value_type));
            return value;
        }
    };
    struct string final : serializable<std::string> {
        static void serialize(const std::string &value, std::deque<uint8_t> &stream) {
            uvarint32::serialize(value.length(), stream);
            for (size_t i = 0; i < value.length(); i++) {
                uint32_t codepoint;

                if (const uint8_t ch = value[i]; ch <= 0x7F) {
                    codepoint = ch;
                } else if (ch <= 0xDF && i + 1 < value.length()) {
                    codepoint |= (ch & 0x1F) << 6;
                    codepoint |= (value[++i] & 0x3F);
                } else if (ch <= 0xEF && i + 2 < value.length()) {
                    codepoint |= (ch & 0x0F) << 12;
                    codepoint |= (value[++i] & 0x3F) << 6;
                    codepoint |= (value[++i] & 0x3F);
                } else if (ch <= 0xF7 && i + 3 < value.length()) {
                    codepoint |= (ch & 0x0F) << 18;
                    codepoint |= (value[++i] & 0x3F) << 12;
                    codepoint |= (value[++i] & 0x3F) << 6;
                    codepoint |= (value[++i] & 0x3F);
                } else {
                    codepoint = 0xFFFD;
                }

                if (codepoint <= 0xFFFF) {
                    uvarint32::serialize(codepoint, stream);
                } else {
                    codepoint -= 0x10000;
                    uvarint32::serialize((codepoint >> 10) + 0xD800, stream);
                    uvarint32::serialize((codepoint & 0x3FF) + 0xDC00, stream);
                }
            }
        }
        static std::string deserialize(std::deque<uint8_t> &stream) {
            const uint32_t length = uvarint32::deserialize(stream);

            std::string result;
            for (size_t i = 0; i < length; i++) {
                if (const uint16_t code = uvarint32::deserialize(stream); code <= 0x7F) {
                    result += static_cast<char>(code);
                } else if (code <= 0x7FF) {
                    result += static_cast<char>(0xC0 | (code >> 6));
                    result += static_cast<char>(0x80 | (code & 0x3F));
                } else {
                    result += static_cast<char>(0xE0 | (code >> 12));
                    result += static_cast<char>(0x80 | (code >> 6) & 0x3F);
                    result += static_cast<char>(0x80 | (code & 0x3F));
                }
            }
            return result;
        }
    };
    struct boolean final : serializable<bool> {
        static void serialize(const bool &value, std::deque<uint8_t> &stream) { stream.push_back((value ? 1 : 0)); }
        static bool deserialize(std::deque<uint8_t> &stream) {
            const uint8_t byte = stream.front();
            stream.pop_front();
            return byte == 1;
        }
    };
    struct uint8_array final : serializable<std::vector<uint8_t>> {
        static void serialize(const std::vector<uint8_t> &value, std::deque<uint8_t> &stream) {
            uvarint32::serialize(value.size(), stream);
            for (uint8_t i: value) {
                stream.push_back(i);
            }
        }
        static std::vector<uint8_t> deserialize(std::deque<uint8_t> &stream) {
            const uint32_t length = uvarint32::deserialize(stream);
            std::vector<uint8_t> value(length);
            for (size_t i = 0; i < length; i++) {
                value[i] = stream.front();
                stream.pop_front();
            }
            return value;
        }
    };
    struct date final : serializable<std::chrono::system_clock::time_point> {
        static void serialize(const std::chrono::system_clock::time_point &value, std::deque<uint8_t> &stream) {
            const int64_t time =
                    std::chrono::duration_cast<std::chrono::milliseconds>(value.time_since_epoch()).count();
            float64::serialize(static_cast<double>(time), stream);
        }

        static std::chrono::time_point<std::chrono::system_clock> deserialize(std::deque<uint8_t> &stream) {
            const double time = float64::deserialize(stream);
            return std::chrono::system_clock::time_point(std::chrono::milliseconds(static_cast<int64_t>(time)));
        }
    };
    template<typename T>
    struct array final : serializable<std::vector<typename T::value_type>> {
        static_assert(std::is_base_of_v<serializable<typename T::value_type>, T>,
                      "T must derive from proto::serializable<typename T::value_type>");
        static void serialize(const std::vector<typename T::value_type> &value, std::deque<uint8_t> &stream) {
            uvarint32::serialize(value.size(), stream);
            for (size_t i = 0; i < value.size(); i++) {
                T::serialize(value[i], stream);
            }
        }
        static std::vector<typename T::value_type> deserialize(std::deque<uint8_t> &stream) {
            const uint32_t length = uvarint32::deserialize(stream);
            std::vector<typename T::value_type> value(length);
            for (size_t i = 0; i < length; i++) {
                value[i] = T::deserialize(stream);
            }
            return value;
        }
    };

    using endpoint = string;
    struct header final : serializable<header> {
        std::string guid;
        std::string encoding;
        uint32_t index{};
        bool final{};

        static void serialize(const header &value, std::deque<uint8_t> &stream) {
            string::serialize(value.guid, stream);
            string::serialize(value.encoding, stream);
            uvarint32::serialize(value.index, stream);
            boolean::serialize(value.final, stream);
        }

        static header deserialize(std::deque<uint8_t> &stream) {
            header header{};
            header.guid = string::deserialize(stream);
            header.encoding = string::deserialize(stream);
            header.index = uvarint32::deserialize(stream);
            header.final = boolean::deserialize(stream);
            return header;
        }
    };
} // namespace proto

namespace net {
    using listener = std::function<void(const proto::header &header, const std::string &serialized_packet)>;

    constexpr size_t max_fragment_size = 2048;
    const std::string encoding = "mcbe-ipc:v3";
    inline std::unordered_map<std::string, std::vector<std::unique_ptr<listener>>> endpoints;
    inline bool scriptevent_debugging = false;

    std::vector<std::string> serialize(const std::deque<uint8_t> &stream, size_t max_size);
    std::deque<uint8_t> deserialize(const std::vector<std::string> &strings);

    void on_script_message_event(const endstone::ScriptMessageEvent &event, const endstone::Plugin &plugin);

    inline std::string generate_id() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint32_t> dis(0, std::numeric_limits<uint32_t>::max());
        const uint32_t rand = dis(gen);

        std::string id{};
        constexpr char hex[] = "0123456789ABCDEF";
        id += hex[rand & 0xF];
        id += hex[(rand >> 4) & 0xF];
        id += hex[(rand >> 8) & 0xF];
        id += hex[(rand >> 12) & 0xF];
        id += hex[(rand >> 16) & 0xF];
        id += hex[(rand >> 20) & 0xF];
        id += hex[(rand >> 24) & 0xF];

        return id;
    }

    inline std::function<void()> create_listener(const std::string &endpoint, const listener &new_listener) {
        endpoints[endpoint].emplace_back(std::make_unique<listener>(new_listener));

        return [&]() {
            auto &listeners = endpoints[endpoint];
            const auto it = std::find_if(listeners.begin(), listeners.end(),
                                         [&](const std::unique_ptr<listener> &l) { return l.get() == &new_listener; });
            if (it != listeners.end()) {
                listeners.erase(it);
            }

            if (listeners.empty()) {
                endpoints.erase(endpoint);
            }
        };
    }

    template<typename S>
    void emit(const std::string &endpoint, const typename S::value_type &value, const endstone::Plugin &plugin) {
        static_assert(std::is_base_of_v<proto::serializable<typename S::value_type>, S>,
                      "S must derive from proto::serializable<typename S::value_type>");

        const std::string guid = generate_id();

        std::deque<uint8_t> endpoint_stream{};
        proto::endpoint::serialize(endpoint, endpoint_stream);

        const std::string endpoint_string = proto::mips::serialize(endpoint_stream);

        const auto emitter = [&](const proto::header &header, const std::string &serialized_packet) {
            std::deque<uint8_t> header_stream{};
            proto::header::serialize(header, header_stream);

            const std::string header_string = proto::mips::serialize(header_stream);

            (void) plugin.getServer().dispatchCommand(plugin.getServer().getCommandSender(),
                                                      "scriptevent " + endpoint_string + ":" + header_string + " " +
                                                              serialized_packet);
        };

        std::deque<uint8_t> packet_stream{};
        S::serialize(value, packet_stream);

        const std::vector<std::string> serialized_packets = serialize(packet_stream, max_fragment_size);
        for (size_t i = 0; i < serialized_packets.size(); i++) {
            const std::string &serialized_packet = serialized_packets[i];

            proto::header header;
            header.guid = guid;
            header.encoding = encoding;
            header.index = static_cast<uint32_t>(i);
            header.final = i == serialized_packets.size() - 1;

            emitter(header, serialized_packet);
        }
    }

    struct buffer_contents {
        uint32_t size = 0;
        std::vector<std::string> serialized_packets{};
        uint32_t accumulated_size = 0;
    };

    template<typename S>
    std::function<void()> listen(const std::string &endpoint,
                                 const std::function<void(const typename S::value_type &value)> &callback) {
        static_assert(std::is_base_of_v<proto::serializable<typename S::value_type>, S>,
                      "S must derive from proto::serializable<typename S::value_type>");

        static std::unordered_map<std::string, buffer_contents> buffer;

        return net::create_listener(
                endpoint, [callback](const proto::header &header, const std::string &serialized_packet) -> void {
                    auto &[size, serialized_packets, accumulated_size] = buffer[header.guid];

                    if (header.final) {
                        size = header.index + 1;
                    }

                    if (header.index >= serialized_packets.size()) {
                        serialized_packets.resize(header.index + 1);
                    }

                    serialized_packets[header.index] = serialized_packet;
                    accumulated_size += header.index + 1;

                    if (size != 0 && accumulated_size == (size * (size + 1)) / 2) {
                        std::deque<uint8_t> packet_stream = deserialize(serialized_packets);
                        const typename S::value_type &value = S::deserialize(packet_stream);

                        callback(value);
                        buffer.erase(header.guid);
                    }
                });
    };
} // namespace net

namespace ipc {
    template<typename S>
    void send(const std::string &channel, const typename S::value_type &value, const endstone::Plugin &plugin) {
        net::emit<S>("ipc:" + channel + ":send", value, plugin);
    }

    template<typename Es, typename Ls>
    std::future<typename Ls::value_type> invoke(const std::string &channel, const typename Ls::value_type &value,
                                                const endstone::Plugin &plugin) {
        net::emit<Es>("ipc:" + channel + ":invoke", value, plugin);

        std::promise<typename Ls::value_type> promise;
        std::future<typename Ls::value_type> future = promise.get_future();

        std::thread([channel, &promise]() {
            auto terminate = std::make_shared<std::function<void()>>();
            *terminate = net::listen<Ls>("ipc:" + channel + ":handle",
                                         [&promise, &terminate](const typename Ls::value_type &r_value) {
                                             promise.set_value(r_value);
                                             (*terminate)();
                                         });
        }).detach();

        return future;
    }

    template<typename S>
    std::function<void()> on(const std::string &channel,
                             const std::function<void(const typename S::value_type &value)> &listener) {
        return net::listen<S>("ipc:" + channel + ":send", listener);
    }

    template<typename S>
    std::function<void()> once(const std::string &channel,
                               const std::function<void(const typename S::value_type &value)> &listener) {
        auto terminate = std::make_shared<std::function<void()>>();
        *terminate =
                net::listen<S>("ipc:" + channel + ":send", [listener, terminate](const typename S::value_type &value) {
                    listener(value);
                    (*terminate)();
                });
        return (*terminate);
    }

    template<typename Ls, typename Es>
    std::function<void()>
    handle(const std::string &channel,
           const std::function<typename Es::value_type(const typename Ls::value_type &value)> &listener,
           const endstone::Plugin &plugin) {
        return net::listen<Ls>("ipc:" + channel + ":invoke",
                               [listener, channel, &plugin](const typename Ls::value_type &l_value) {
                                   const typename Es::value_type &result = listener(l_value);
                                   net::emit<Es>("ipc:" + channel + ":handle", result, plugin);
                               });
    }
} // namespace ipc


/* ipc.cpp MOVED INLINE: */

inline std::vector<std::string> split(const std::string &s, const std::string &delim) {
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
    inline std::string serialize(const std::deque<uint8_t> &stream) {
        constexpr char hex[] = "0123456789ABCDEF";
        std::string str = "(0x";
        for (const uint8_t v: stream) {
            str += hex[(v >> 4) & 0xF];
            str += hex[v & 0xF];
        }
        str += ")";
        return str;
    };

    inline std::deque<uint8_t> deserialize(const std::string &str) {
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
    inline std::vector<std::string> serialize(const std::deque<uint8_t> &stream,
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

    inline std::deque<uint8_t> deserialize(const std::vector<std::string> &strings) {
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

    inline void on_script_message_event(const endstone::ScriptMessageEvent &event, const endstone::Plugin &plugin) {
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