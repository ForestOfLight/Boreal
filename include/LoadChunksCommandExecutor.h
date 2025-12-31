#pragma once

#include <endstone/command/command_executor.h>
#include <string>
#include "PlayersTickLevelChunks.h"

class LoadChunksCommandExecutor : public endstone::CommandExecutor {
public:
    bool onCommand(endstone::CommandSender &sender, const endstone::Command &command,
             const std::vector<std::string> &args) override {
        bool shouldLoadChunks = true;
        if (!args.empty())
            shouldLoadChunks = args[0] == "true";
        const int64_t playerId = sender.asPlayer()->getId();
        if (shouldLoadChunks)
            PlayersTickLevelChunks::startLoadingChunks(playerId);
        else
            PlayersTickLevelChunks::stopLoadingChunks(playerId);
        sender.sendMessage("Chunkloading set to " + args[0]);
        return true;
    }
};