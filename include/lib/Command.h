#pragma once

#include <string>
#include <vector>

#include "lib/Rule.h"

class Command {
    public:
        Command(std::string name, std::string description, std::string usage, std::vector<std::string> args = {},
              std::vector<Rule> contingentRules = {}, bool adminOnly = false, std::vector<std::string> helpEntries = {},
              bool helpHidden = false);
        ~Command();

        std::string getName();
        std::string getDescription();
        std::string getUsage();
        std::vector<std::string> getArgs();
        std::vector<Rule> getContingentRules();
        bool isAdminOnly();
        std::vector<std::string> getHelpEntries();
        bool isHelpHidden();

    private:
        std::string name;
        std::string description;
        std::string usage;
        std::vector<std::string> args;
        std::vector<Rule> contingentRules;
        bool adminOnly;
        std::vector<std::string> helpEntries;
        bool helpHidden;
};
