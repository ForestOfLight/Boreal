#pragma once

#include <endstone/command/command_executor.h>
#include <string>

#include "PistonPushLimit.h"

class PistonPushLimitCommandExecutor : public endstone::CommandExecutor {
public:
    bool onCommand(endstone::CommandSender &sender, const endstone::Command &command,
             const std::vector<std::string> &args) override {
        if (args.empty()) {
            int current = PistonPushLimit::getPistonPushLimit();
            sender.sendMessage(fmt::format("Piston push limit is {}.", current));
            return true;
        }

        int newPushLimit = VANILLA_PISTON_PUSH_LIMIT;
        try {
            newPushLimit = std::stoi(args[0]);
        } catch (const std::exception &) {
            sender.sendMessage("Invalid number.");
            return false;
        }
        if (newPushLimit < 0)
            newPushLimit = 0;
        PistonPushLimit::setPistonPushLimit(newPushLimit);
        sender.sendMessage(fmt::format("Piston push limit is now {}.", newPushLimit));
        return true;
    }
};