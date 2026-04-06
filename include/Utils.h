#pragma once

#include <EnTT.h>

class Utils {
public:
    static int64_t getUniqueIDFromActorPtr(void *actor) {
        EntityContext player = reinterpret_cast<Actor*>(actor)->getEntity();
        return player.tryGetComponent<ActorUniqueIDComponent>()->mActorUniqueID;
    }
};
