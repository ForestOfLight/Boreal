#pragma once

#include <cstdint>
#include "hook.h"

extern void* getBaseAddress();

class Utils {
public:
    static int64_t getUniqueIDFromActorPtr(void *actor) {
        using GetActorUniqueIDComponentFn = int64_t *(__fastcall*)(void *actor);
        void *baseAddress = getBaseAddress();
        GetActorUniqueIDComponentFn getActorUniqueIDComponent =
            (GetActorUniqueIDComponentFn)((char*)baseAddress + 59202864); // address of "Actor::getOrCreateUniqueID"
        int64_t *actorUniqueIDComponentPtr = getActorUniqueIDComponent(actor);
        if (actorUniqueIDComponentPtr) {
            return *actorUniqueIDComponentPtr;
        }
        return -1;
    }
};
