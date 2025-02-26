#pragma once

#include <string>
#include <vector>

#include "lib/extension.ipc.h"

class Command {
    public:
        Command(std::string name, std::string description, std::string usage, std::vector<Arg> args,
                 std::vector<std::string> contingentRules, bool adminOnly, std::vector<HelpEntry> helpEntries,
                 bool helpHidden);
        ~Command();

        std::string getName();
        std::string getDescription();
        std::string getUsage();
        std::vector<Arg> getArgs();
        std::vector<std::string> getContingentRules();
        bool isAdminOnly();
        std::vector<HelpEntry> getHelpEntries();
        bool isHelpHidden();

    private:
        std::string name;
        std::string description;
        std::string usage;
        std::vector<Arg> args;
        std::vector<std::string> contingentRules;
        bool adminOnly;
        std::vector<HelpEntry> helpEntries;
        bool helpHidden;
};

inline Command::Command(std::string name, std::string description, std::string usage, std::vector<Arg> args,
    std::vector<std::string> contingentRules, bool adminOnly, std::vector<HelpEntry> helpEntries, bool helpHidden) {
    this->name = name;
    this->description = description;
    this->usage = usage;
    this->args = args;
    this->contingentRules = contingentRules;
    this->adminOnly = adminOnly;
    this->helpEntries = helpEntries;
    this->helpHidden = helpHidden;
}

inline Command::~Command() {
    this->args.clear();
    this->contingentRules.clear();
    this->helpEntries.clear();
}

inline std::string Command::getName() {
    return this->name;
}

inline std::string Command::getDescription() {
    return this->description;
}

inline std::string Command::getUsage() {
    return this->usage;
}

inline std::vector<Arg> Command::getArgs() {
    return this->args;
}

inline std::vector<std::string> Command::getContingentRules() {
    return this->contingentRules;
}

inline bool Command::isAdminOnly() {
    return this->adminOnly;
}

inline std::vector<HelpEntry> Command::getHelpEntries() {
    return this->helpEntries;
}

inline bool Command::isHelpHidden() {
    return this->helpHidden;
}
