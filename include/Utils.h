#pragma once

#include <EnTT.h>
#include <endstone/player.h>
#include <endstone/server.h>

class Utils {
public:
    static int64_t getUniqueIDFromActorPtr(void *actor) {
        EntityContext player = reinterpret_cast<Actor*>(actor)->getEntity();
        return player.tryGetComponent<ActorUniqueIDComponent>()->mActorUniqueID;
    }

    static endstone::Player *getPlayerFromUniqueID(endstone::Server *server, int64_t uniqueId) {
        for (auto *player : server->getOnlinePlayers()) {
            if (player->getId() == uniqueId)
                return player;
        }
        return nullptr;
    }
};
