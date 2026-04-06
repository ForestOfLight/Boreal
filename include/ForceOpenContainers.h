#pragma once
#include "Utils.h"

bool (*_canOpenThis)(void *chestBlockActor, void *blockSource);

class ForceOpenContainers {
public:
    static endstone::Logger *logger;
    static bool isEnabled;

    static void enable() {
        isEnabled = true;
    }

    static void disable() {
        isEnabled = false;
    }

    static void hook(void *baseAddress, funchook_t *funchook);
};

inline bool ForceOpenContainers::isEnabled = false;
inline endstone::Logger *ForceOpenContainers::logger;

bool canOpenThisHook(void *chestBlockActor, void *blockSource) {
    if (ForceOpenContainers::isEnabled) {
        return true;
    }
    return _canOpenThis(chestBlockActor, blockSource);
}

inline void ForceOpenContainers::hook(void *baseAddress, funchook_t *funchook) {
#ifdef __GNUC__
    void *_canOpenThisAddr = (char *)baseAddress + ?; // address of "?"
#else
    void *_canOpenThisAddr = (char *)baseAddress + 68241344; // address of "ChestBlockActor::_canOpenThis"
#endif
    _canOpenThis = (bool(*)(void*, void*))_canOpenThisAddr;
    int errorCode = funchook_prepare(funchook, (void **)&_canOpenThis, canOpenThisHook);
    if (errorCode) {
        logger->error("Failed to prepare hook: {}", funchook_error_message(funchook));
    }
}

