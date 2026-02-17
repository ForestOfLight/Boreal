#pragma once

#include <endstone/command/command_executor.h>
#include <string>
#include "ForceOpenContainers.h"

class ForceOpenCommandExecutor : public endstone::CommandExecutor {
public:
    bool onCommand(endstone::CommandSender &sender, const endstone::Command &command,
             const std::vector<std::string> &args) override {
        bool shouldForceOpen = true;
        if (!args.empty())
            shouldForceOpen = args[0] == "true";
    if (shouldForceOpen) {
            ForceOpenContainers::enable();
            sender.sendMessage("Players can now force open containers.");
        } else {
            ForceOpenContainers::disable();
            sender.sendMessage("Players can no longer force open containers.");
        }
        return true;
    }
};