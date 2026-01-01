#pragma once

#include <cstdint>
#include "hook.h"

extern void* getBaseAddress();

class Utils {
public:
    static int64_t getUniqueIDFromActorPtr(void *actor) {
        using GetActorUniqueIDComponentFn = int64_t *(__fastcall*)(void *actor, int64_t *out);
        void *baseAddress = getBaseAddress();
        GetActorUniqueIDComponentFn getActorUniqueIDComponent =
            (GetActorUniqueIDComponentFn)((char*)baseAddress + 57003088); // address of "Actor::getPersistentComponent<ActorUniqueIDComponent>"
        int64_t actorUniqueIDComponentPtr = 0LL;
        getActorUniqueIDComponent(actor, &actorUniqueIDComponentPtr);
        if (actorUniqueIDComponentPtr) {
            return *reinterpret_cast<int64_t *>(actorUniqueIDComponentPtr);
        }
        return -1;
    }
};
