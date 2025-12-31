#pragma once

#include <endstone/command/command_executor.h>
#include <string>

class FlySpeedCommandExecutor : public endstone::CommandExecutor {
    float VANILLA_FLY_SPEED = 0.05f;
public:
    bool onCommand(endstone::CommandSender &sender, const endstone::Command &command,
             const std::vector<std::string> &args) override {
        float flightSpeedMultiplier = 1;
        if (!args.empty())
            flightSpeedMultiplier = std::stof(args[0]);
        sender.sendMessage("Flight speed multiplier set to " + args[0] + "x");
        sender.asPlayer()->setFlySpeed(flightSpeedMultiplier * VANILLA_FLY_SPEED);
        return true;
    }
};