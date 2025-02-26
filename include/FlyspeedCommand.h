#pragma once

#include <endstone/command/command_executor.h>
#include <string>
#include <endstone/player.h>

class FlyspeedCommandExecutor : public endstone::CommandExecutor {
public:
    bool onCommand(endstone::CommandSender &sender, const endstone::Command &command,
                 const std::vector<std::string> &args) override {
        if (args.size() == 0) {
            sender.sendMessage("Flyspeed set to default");
            sender.asPlayer()->setFlySpeed(0.05f);
            return true;
        }
        sender.sendMessage("Flyspeed set to: " + args[0] + "x");
        sender.asPlayer()->setFlySpeed(std::stof(args[0]) * 0.05);
        return true;
    }
};