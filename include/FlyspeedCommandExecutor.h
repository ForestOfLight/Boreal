#pragma once

#include <endstone/command/command_executor.h>
#include <string>

class FlySpeedCommandExecutor : public endstone::CommandExecutor {
    float VANILLA_FLY_SPEED = 0.05f;
public:
    bool onCommand(endstone::CommandSender &sender, const endstone::Command &command,
             const std::vector<std::string> &args) override {
        float flightSpeedMultiplier = sender.asPlayer()->getFlySpeed() * 20;
        if (!args.empty()) {
            flightSpeedMultiplier = std::stof(args[0]) >= 0 ? std::stof(args[0]) : 1;
            sender.sendMessage("Flight speed multiplier set to " + fmt::format("{}", flightSpeedMultiplier) + "x");
            sender.asPlayer()->setFlySpeed(flightSpeedMultiplier * VANILLA_FLY_SPEED);
        }
        else sender.sendMessage("Current flight speed multiplier is " + fmt::format("{}", flightSpeedMultiplier)+ "x");
        return true;
    }
};