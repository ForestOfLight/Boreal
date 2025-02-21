#pragma once

#include <string>
#include <vector>

#include "lib/Rule.h"

struct Arg {
    std::string type;
    std::string name;
};

struct HelpEntry {
    std::string usage;
    std::string description;
};

class Command {
    public:
        Command(std::string name, std::string description, std::string usage, std::vector<Arg> args = {},
              std::vector<std::string> contingentRules = {}, bool adminOnly = false, std::vector<HelpEntry> helpEntries = {},
              bool helpHidden = false);
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
