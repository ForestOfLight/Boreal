#include "classes/PistonPushLimit.h"

int PistonPushLimit::pistonPushLimit = VANILLA_PISTON_PUSH_LIMIT;
std::vector<MemoryPatch> PistonPushLimit::patches = {};
endstone::Logger *PistonPushLimit::logger = nullptr;

void PistonPushLimit::scanFunction(void *fnStart, size_t searchSize) {
    auto matches = PatternScanner::scan(fnStart, searchSize, kSearchPattern, sizeof(kSearchPattern));
    for (const auto &match : matches) {
        void *limitByte = reinterpret_cast<uint8_t *>(match.address) + kLimitByteOffset;
        if (*reinterpret_cast<uint8_t *>(limitByte) != VANILLA_PISTON_PUSH_LIMIT) {
            logger->warning("Piston patch site at {:p} has unexpected value {:02x}, skipping",
                limitByte, *reinterpret_cast<uint8_t *>(limitByte));
        } else {
            patches.emplace_back(limitByte, static_cast<uint8_t>(pistonPushLimit));
        }
    }
}

void PistonPushLimit::setPistonPushLimit(int newLimit) {
    pistonPushLimit = newLimit;
    for (auto &patch : patches)
        patch.setValue(static_cast<uint8_t>(newLimit));
}

int PistonPushLimit::getPistonPushLimit() {
    return pistonPushLimit;
}

void PistonPushLimit::install(void *baseAddress) {
#ifdef __GNUC__
    void *_checkAttachedBlocksAddr = (char *)baseAddress + ?; // address of "?"
    void *_attachedBlockWalkerAddr = (char *)baseAddress + ?; // address of "?"
    size_t _checkAttachedBlocksSize = 0;
    size_t _attachedBlockWalkerSize = 0;
#else
    void *_checkAttachedBlocksAddr = (char *)baseAddress + 0x41D68C0; // address of "PistonBlockActor::_checkAttachedBlocks"
    size_t _checkAttachedBlocksSize = 0x25A;
    void *_attachedBlockWalkerAddr = (char *)baseAddress + 0x41d6b20; // address of "PistonBlockActor::_attachedBlockWalker"
    size_t _attachedBlockWalkerSize = 0x50E;
#endif
    scanFunction(_checkAttachedBlocksAddr, _checkAttachedBlocksSize);
    scanFunction(_attachedBlockWalkerAddr, _attachedBlockWalkerSize);

    if (patches.empty()) {
        logger->error("Failed to find any piston push limit patch sites");
        return;
    }

    for (auto &patch : patches) {
        if (!patch.apply())
            logger->error("Failed to apply piston push limit patch at {:p}", patch.getAddress());
    }
}

void PistonPushLimit::uninstall() {
    for (auto &patch : patches) {
        if (!patch.restore())
            logger->error("Failed to restore piston push limit patch at {:p}", patch.getAddress());
    }
    patches.clear();
}

