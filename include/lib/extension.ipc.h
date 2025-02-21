#include "ipc.h"

#include <string>
#include <deque>
#include <vector>

struct Ready final : proto::serializable<Ready> {
    static void serialize(const Ready &value, std::deque<uint8_t> &stream) {
    }

    static Ready deserialize(std::deque<uint8_t> &stream) {
        return Ready{};
    }
};

struct RegisterExtension final : proto::serializable<RegisterExtension> {
    std::string name;
    std::string version;
    std::string author;
    std::string description;
    bool isEndstone;

    static void serialize(const RegisterExtension &value, std::deque<uint8_t> &stream) {
        proto::string::serialize(value.name, stream);
        proto::string::serialize(value.version, stream);
        proto::string::serialize(value.author, stream);
        proto::string::serialize(value.description, stream);
        proto::boolean::serialize(value.isEndstone, stream);
    }

    static RegisterExtension deserialize(std::deque<uint8_t> &stream) {
        RegisterExtension value{};
        value.name = proto::string::deserialize(stream);
        value.version = proto::string::deserialize(stream);
        value.author = proto::string::deserialize(stream);
        value.description = proto::string::deserialize(stream);
        value.isEndstone = proto::boolean::deserialize(stream);
        return value;
    }
};

struct Arg final : proto::serializable<Arg> {
    std::string type;
    std::string name;

    static void serialize(const Arg &value, std::deque<uint8_t> &stream) {
        proto::string::serialize(value.type, stream);
        proto::string::serialize(value.name, stream);
    }

    static Arg deserialize(std::deque<uint8_t> &stream) {
        Arg value{};
        value.type = proto::string::deserialize(stream);
        value.name = proto::string::deserialize(stream);
        return value;
    }
};

struct HelpEntry final : proto::serializable<HelpEntry> {
    std::string usage;
    std::string description;

    static void serialize(const HelpEntry &value, std::deque<uint8_t> &stream) {
        proto::string::serialize(value.usage, stream);
        proto::string::serialize(value.description, stream);
    }

    static HelpEntry deserialize(std::deque<uint8_t> &stream) {
        HelpEntry value{};
        value.usage = proto::string::deserialize(stream);
        value.description = proto::string::deserialize(stream);
        return value;
    }
};

struct RegisterCommand final : proto::serializable<RegisterCommand> {
    std::string name;
    std::string description;
    std::string usage;
    std::vector<Arg> args;
    std::vector<std::string> contingentRules;
    bool adminOnly;
    std::vector<HelpEntry> helpEntries;
    bool helpHidden;
    std::string extensionName;

    static void serialize(const RegisterCommand &value, std::deque<uint8_t> &stream) {
        proto::string::serialize(value.name, stream);
        proto::string::serialize(value.description, stream);
        proto::string::serialize(value.usage, stream);
        proto::array<Arg>::serialize(value.args, stream);
        proto::array<proto::string>::serialize(value.contingentRules, stream);
        proto::boolean::serialize(value.adminOnly, stream);
        proto::array<HelpEntry>::serialize(value.helpEntries, stream);
        proto::boolean::serialize(value.helpHidden, stream);
        proto::string::serialize(value.extensionName, stream);
    }

    static RegisterCommand deserialize(std::deque<uint8_t> &stream) {
        RegisterCommand value{};
        value.name = proto::string::deserialize(stream);
        value.description = proto::string::deserialize(stream);
        value.usage = proto::string::deserialize(stream);
        value.args = proto::array<Arg>::deserialize(stream);
        value.contingentRules = proto::array<proto::string>::deserialize(stream);
        value.adminOnly = proto::boolean::deserialize(stream);
        value.helpEntries = proto::array<HelpEntry>::deserialize(stream);
        value.helpHidden = proto::boolean::deserialize(stream);
        value.extensionName = proto::string::deserialize(stream);
        return value;
    }
};

struct RegisterRule final : proto::serializable<RegisterRule> {
    std::string identifier;
    std::string description;
    std::vector<std::string> contingentRules;
    std::vector<std::string> independentRules;
    std::string extensionName;

    static void serialize(const RegisterRule &value, std::deque<uint8_t> &stream) {
        proto::string::serialize(value.identifier, stream);
        proto::string::serialize(value.description, stream);
        proto::array<proto::string>::serialize(value.contingentRules, stream);
        proto::array<proto::string>::serialize(value.independentRules, stream);
        proto::string::serialize(value.extensionName, stream);
    }

    static RegisterRule deserialize(std::deque<uint8_t> &stream) {
        RegisterRule value{};
        value.identifier = proto::string::deserialize(stream);
        value.description = proto::string::deserialize(stream);
        value.contingentRules = proto::array<proto::string>::deserialize(stream);
        value.independentRules = proto::array<proto::string>::deserialize(stream);
        value.extensionName = proto::string::deserialize(stream);
        return value;
    }
};

struct RuleValueRequest final : proto::serializable<RuleValueRequest> {
    std::string ruleID;

    static void serialize(const RuleValueRequest &value, std::deque<uint8_t> &stream) {
        proto::string::serialize(value.ruleID, stream);
    }

    static RuleValueRequest deserialize(std::deque<uint8_t> &stream) {
        RuleValueRequest value{};
        value.ruleID = proto::string::deserialize(stream);
        return value;
    }
};

struct RuleValueResponse final : proto::serializable<RuleValueResponse> {
    bool value;

    static void serialize(const RuleValueResponse &value, std::deque<uint8_t> &stream) {
        proto::boolean::serialize(value.value, stream);
    }

    static RuleValueResponse deserialize(std::deque<uint8_t> &stream) {
        RuleValueResponse value{};
        value.value = proto::boolean::deserialize(stream);
        return value;
    }
};

struct RuleValueSet final : proto::serializable<RuleValueSet> {
    std::string ruleID;
    bool value;

    static void serialize(const RuleValueSet &value, std::deque<uint8_t> &stream) {
        proto::string::serialize(value.ruleID, stream);
        proto::boolean::serialize(value.value, stream);
    }

    static RuleValueSet deserialize(std::deque<uint8_t> &stream) {
        RuleValueSet value{};
        value.ruleID = proto::string::deserialize(stream);
        value.value = proto::boolean::deserialize(stream);
        return value;
    }
};

struct CommandCallbackRequest final : proto::serializable<CommandCallbackRequest> {
    std::string commandName;
    std::string senderName;
    std::vector<std::string> args;

    static void serialize(const CommandCallbackRequest &value, std::deque<uint8_t> &stream) {
        proto::string::serialize(value.commandName, stream);
        proto::string::serialize(value.senderName, stream);
        proto::array<proto::string>::serialize(value.args, stream);
    }

    static CommandCallbackRequest deserialize(std::deque<uint8_t> &stream) {
        CommandCallbackRequest value{};
        value.commandName = proto::string::deserialize(stream);
        value.senderName = proto::string::deserialize(stream);
        value.args = proto::array<proto::string>::deserialize(stream);
        return value;
    }
};

struct CommandPrefixRequest final : proto::serializable<CommandPrefixRequest> {
    static void serialize(const CommandPrefixRequest &value, std::deque<uint8_t> &stream) {
    }

    static CommandPrefixRequest deserialize(std::deque<uint8_t> &stream) {
        return CommandPrefixRequest{};
    }
};

struct CommandPrefixResponse final : proto::serializable<CommandPrefixResponse> {
    std::string prefix;

    static void serialize(const CommandPrefixResponse &value, std::deque<uint8_t> &stream) {
        proto::string::serialize(value.prefix, stream);
    }

    static CommandPrefixResponse deserialize(std::deque<uint8_t> &stream) {
        CommandPrefixResponse value{};
        value.prefix = proto::string::deserialize(stream);
        return value;
    }
};
