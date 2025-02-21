#pragma once

#include <string>
#include <vector>

#include "lib/Rule.h"
#include "lib/extension.ipc.h"

class Command {
    public:
        Command(std::string name, std::string description, std::string usage, std::vector<Arg> args,
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

        ~Command() {
            this->args.clear();
            this->contingentRules.clear();
            this->helpEntries.clear();
        }

        std::string getName() {
            return this->name;
        }

        std::string getDescription() {
            return this->description;
        }

        std::string getUsage() {
            return this->usage;
        }

        std::vector<Arg> getArgs() {
            return this->args;
        }

        std::vector<std::string> getContingentRules() {
            return this->contingentRules;
        }

        bool isAdminOnly() {
            return this->adminOnly;
        }

        std::vector<HelpEntry> getHelpEntries() {
            return this->helpEntries;
        }

        bool isHelpHidden() {
            return this->helpHidden;
        }

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
