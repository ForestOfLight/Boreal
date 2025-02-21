#include "lib/Command.h"

Command::Command(std::string name, std::string description, std::string usage, std::vector<Arg> args,
                 std::vector<std::string> contingentRules, bool adminOnly, std::vector<HelpEntry> helpEntries,
                 bool helpHidden) {
    this->name = name;
    this->description = description;
    this->usage = usage;
    this->args = args;
    this->contingentRules = contingentRules;
    this->adminOnly = adminOnly;
    this->helpEntries = helpEntries;
    this->helpHidden = helpHidden;
}

Command::~Command() {
    this->args.clear();
    this->contingentRules.clear();
    this->helpEntries.clear();
}

std::string Command::getName() {
    return this->name;
}

std::string Command::getDescription() {
    return this->description;
}

std::string Command::getUsage() {
    return this->usage;
}

std::vector<Arg> Command::getArgs() {
    return this->args;
}

std::vector<std::string> Command::getContingentRules() {
    return this->contingentRules;
}

bool Command::isAdminOnly() {
    return this->adminOnly;
}

std::vector<HelpEntry> Command::getHelpEntries() {
    return this->helpEntries;
}

bool Command::isHelpHidden() {
    return this->helpHidden;
}

#include "lib/Rule.h"