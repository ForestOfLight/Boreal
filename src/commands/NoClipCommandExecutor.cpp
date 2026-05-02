#include "commands/NoClipCommandExecutor.h"

#include "classes/PlayerAbilities.h"

bool NoClipCommandExecutor::onCommand(endstone::CommandSender &sender, const endstone::Command &command,
                                      const std::vector<std::string> &args) {
    if (!PlayerAbilities::isInstalled()) {
        sender.sendErrorMessage("NoClip is unavailable: ability slot offset could not be determined for this server version.");
        return false;
    }
    const int64_t playerId = sender.asPlayer()->getId();
    if (args[0] == "true") {
        PlayerNoClip::enable(playerId);
        sender.sendMessage("NoClip enabled. NoClip will be active while flying in Creative mode.");
    } else if (args[0] == "false") {
        PlayerNoClip::disable(playerId);
        sender.sendMessage("NoClip disabled.");
    }
    return true;
}

