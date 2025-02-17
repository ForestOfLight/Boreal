#pragma once

#include <string>
#include <unordered_map>

#include "Command.h"
#include "Rule.h"
#include "lib/ipc.h"

struct CanopyIntegrationPacket final : proto::serializable<CanopyIntegrationPacket> {
    std::string name;
    std::string version;
    std::string author;
    std::string description;

    static void serialize(const CanopyIntegrationPacket &value, std::deque<uint8_t> &stream) {
        proto::string::serialize(value.name, stream);
        proto::string::serialize(value.version, stream);
        proto::string::serialize(value.author, stream);
        proto::string::serialize(value.description, stream);
    }

    static CanopyIntegrationPacket deserialize(std::deque<uint8_t> &stream) {
        CanopyIntegrationPacket value{};
        value.name = proto::string::deserialize(stream);
        value.version = proto::string::deserialize(stream);
        value.author = proto::string::deserialize(stream);
        value.description = proto::string::deserialize(stream);
        return value;
    }
};

class CanopyIntegration {
    public:
        std::string id;
        std::string name;
        std::string version;
        std::string author;
        std::string description;

        CanopyIntegration(std::string name, std::string version, std::string author, std::string description);
        ~CanopyIntegration();

        void addCommand(Command command);
        void addRule(Rule rule);
        bool getRuleValue(std::string ruleID);
        void setRuleValue(std::string ruleID, bool value);

    private:
        std::unordered_map<std::string, Command> commands;
        std::unordered_map<std::string, Rule> rules;
        bool isRegistrationReady;

        std::string makeID(std::string name);
        void registerIntegration();
        void registerCommand(Command command);
        void registerRule(Rule rule);
        void handleRuleValueRequests();
        void handleRuleValueSetters();
};
