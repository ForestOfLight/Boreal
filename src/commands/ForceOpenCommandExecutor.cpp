#include "commands/ForceOpenCommandExecutor.h"

bool ForceOpenCommandExecutor::onCommand(endstone::CommandSender &sender, const endstone::Command &command,
                                          const std::vector<std::string> &args) {
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

