#pragma once

#include <endstone/command/command_executor.h>
#include <string>
#include "../classes/ForceOpenContainers.h"

class ForceOpenCommandExecutor : public endstone::CommandExecutor {
public:
    bool onCommand(endstone::CommandSender &sender, const endstone::Command &command,
             const std::vector<std::string> &args) override;
};