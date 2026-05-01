#pragma once

#include <endstone/command/command_executor.h>
#include <string>
#include "../classes/PlayerNoClip.h"

class NoClipCommandExecutor : public endstone::CommandExecutor {
public:
    bool onCommand(endstone::CommandSender &sender, const endstone::Command &command,
                   const std::vector<std::string> &args) override {
        if (!PlayerNoClip::ready) {
            sender.sendErrorMessage("NoClip is unavailable: ability slot offset could not be determined for this server version.");
            return false;
        }
        const int64_t playerId = sender.asPlayer()->getId();
        if (args[0] == "enable") {
            PlayerNoClip::enable(playerId);
            sender.sendMessage("NoClip enabled. NoClip will be active while in Creative and flying.");
        } else if (args[0] == "disable") {
            PlayerNoClip::disable(playerId);
            sender.sendMessage("NoClip disabled.");
        }
        return true;
    }
};
