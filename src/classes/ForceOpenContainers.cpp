#include "classes/ForceOpenContainers.h"

bool ForceOpenContainers::isEnabled = false;
endstone::Logger *ForceOpenContainers::logger = nullptr;

// Internal trampoline pointer — not exposed in the header.
static bool (*_canOpenThis)(void *chestBlockActor, void *blockSource) = nullptr;

static bool canOpenThisHook(void *chestBlockActor, void *blockSource) {
    if (ForceOpenContainers::isEnabled)
        return true;
    return _canOpenThis(chestBlockActor, blockSource);
}

void ForceOpenContainers::enable() {
    isEnabled = true;
}

void ForceOpenContainers::disable() {
    isEnabled = false;
}

void ForceOpenContainers::hook(void *baseAddress, funchook_t *funchook) {
#ifdef __GNUC__
    void *_canOpenThisAddr = (char *)baseAddress + ?; // address of "?"
#else
    void *_canOpenThisAddr = (char *)baseAddress + 68241344; // address of "ChestBlockActor::_canOpenThis"
#endif
    _canOpenThis = (bool(*)(void*, void*))_canOpenThisAddr;
    int errorCode = funchook_prepare(funchook, (void **)&_canOpenThis, canOpenThisHook);
    if (errorCode)
        logger->error("Failed to prepare hook: {}", funchook_error_message(funchook));
}

