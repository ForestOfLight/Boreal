#pragma once

#include <endstone/command/command_executor.h>
#include <endstone/player.h>
#include <string>

class FlySpeedCommandExecutor : public endstone::CommandExecutor {
    static constexpr float VANILLA_FLY_SPEED = 0.05f;
public:
    bool onCommand(endstone::CommandSender &sender, const endstone::Command &command,
             const std::vector<std::string> &args) override;
};