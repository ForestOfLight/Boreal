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
