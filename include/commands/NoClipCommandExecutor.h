#pragma once

#include <endstone/command/command_executor.h>
#include <string>
#include "../classes/PlayerNoClip.h"

class NoClipCommandExecutor : public endstone::CommandExecutor {
public:
    bool onCommand(endstone::CommandSender &sender, const endstone::Command &command,
                   const std::vector<std::string> &args) override;
};
