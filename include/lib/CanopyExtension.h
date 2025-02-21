#pragma once

#include <string>
#include <unordered_map>

#include "Command.h"
#include "Rule.h"
#include "lib/ipc.h"

class CanopyExtension {
    public:
        std::string id;
        std::string name;
        std::string version;
        std::string author;
        std::string description;
        endstone::Plugin &plugin;

        CanopyExtension(std::string name, std::string version, std::string author, std::string description, endstone::Plugin &plugin);
        ~CanopyExtension();

        void addCommand(Command command);
        void addRule(Rule rule);
        bool getRuleValue(std::string ruleID);
        void setRuleValue(std::string ruleID, bool value);

    private:
        std::unordered_map<std::string, Command> commands;
        std::unordered_map<std::string, Rule> rules;
        bool isRegistrationReady;

        std::string makeID(std::string name);
        void registerExtension();
        void registerCommand(Command command);
        void registerRule(Rule rule);
        void handleRuleValueRequests();
        void handleRuleValueSetters();
};
