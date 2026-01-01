#pragma once
#include <funchook.h>

bool (*_checkAttachedBlocks)(void *pistonBlockActor, void *blockSource);

#define VANILLA_PISTON_PUSH_LIMIT 12

class PistonPushLimit {
    static int pistonPushLimit;

public:
    static endstone::Logger *logger;

    static void setPistonPushLimit(const int newPushLimit) {
        pistonPushLimit = newPushLimit;
    }

    static int getPistonPushLimit() {
        return pistonPushLimit;
    }

    static void hook(void *baseAddress, funchook_t *funchook);
};

inline int PistonPushLimit::pistonPushLimit = VANILLA_PISTON_PUSH_LIMIT;
inline endstone::Logger *PistonPushLimit::logger;

const int64_t mAttachedBlocksOffset = 0xB0; // offset of "std::vector<BlockPos> mAttachedBlocks" in PistonBlockActor

bool checkAttachedBlocksHook(void *pistonBlockActor, void *blockSource) {
    _checkAttachedBlocks(pistonBlockActor, blockSource);
    auto *vecBase = (char *)pistonBlockActor + mAttachedBlocksOffset;
    char *begin = *reinterpret_cast<char **>(vecBase);
    char *end = *reinterpret_cast<char **>(vecBase + sizeof(void *));
    auto attachedBlocksCount = (end - begin) / 12; // BlockPos size = 12 bytes
    if (attachedBlocksCount <= PistonPushLimit::getPistonPushLimit())
        return true;
    return false;
}

inline void PistonPushLimit::hook(void *baseAddress, funchook_t *funchook) {
#ifdef __GNUC__
    void *_checkAttachedBlocksAddr = (char *)baseAddress + ?; // address of "?"
#else
    void *_checkAttachedBlocksAddr = (char *)baseAddress + 71861632; // address of "PistonBlockActor::_checkAttachedBlocks"
#endif
    _checkAttachedBlocks = (bool(*)(void*, void*))_checkAttachedBlocksAddr;
    int errorCode = funchook_prepare(funchook, (void **)&_checkAttachedBlocks, checkAttachedBlocksHook);
    if (errorCode) {
        logger->error("Failed to prepare hook: {}", funchook_error_message(funchook));
    }
}

