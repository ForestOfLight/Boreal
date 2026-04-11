#pragma once
#include "MemoryPatch.h"
#include "PatternScanner.h"
#include <vector>
#include <cstdint>

#define VANILLA_PISTON_PUSH_LIMIT 12

class PistonPushLimit {
    static int pistonPushLimit;
    static std::vector<MemoryPatch> patches;

    static constexpr uint8_t kSearchPattern[] = {
        0x48, 0x0F, 0xAF, 0xC1, // imul rax, rcx
        0x48, 0x83, 0xF8, 0x0C  // cmp rax, 0Ch -- the <= 12 comparison
    };
    static constexpr size_t kLimitByteOffset = 7; // offset of 0x0C within pattern

    static void scanFunction(void *fnStart, size_t searchSize) {
        auto matches = PatternScanner::scan(fnStart, searchSize, kSearchPattern, sizeof(kSearchPattern));
        for (const auto& match : matches) {
            void *limitByte = reinterpret_cast<uint8_t *>(match.address) + kLimitByteOffset;
            if (*reinterpret_cast<uint8_t *>(limitByte) != VANILLA_PISTON_PUSH_LIMIT) {
                logger->warning("Piston patch site at {:p} has unexpected value {:02x}, skipping",
                    limitByte, *reinterpret_cast<uint8_t *>(limitByte));
            } else {
                patches.emplace_back(limitByte, static_cast<uint8_t>(pistonPushLimit));
            }
        }
    }

public:
    static endstone::Logger *logger;

    static void setPistonPushLimit(int newLimit) {
        pistonPushLimit = newLimit;
        for (auto &patch : patches)
            patch.setValue(static_cast<uint8_t>(newLimit));
    }

    static int getPistonPushLimit() {
        return pistonPushLimit;
    }

    static void install(void *baseAddress) {
#ifdef __GNUC__
        void *_checkAttachedBlocksAddr = (char *)baseAddress + ?; // address of "?"
        void *_attachedBlockWalkerAddr = (char *)baseAddress + ?; // address of "?"
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

    static void uninstall() {
        for (auto &patch : patches) {
            if (!patch.restore())
                logger->error("Failed to restore piston push limit patch at {:p}", patch.getAddress());
        }
        patches.clear();
    }
};

inline int PistonPushLimit::pistonPushLimit = VANILLA_PISTON_PUSH_LIMIT;
inline std::vector<MemoryPatch> PistonPushLimit::patches;
inline endstone::Logger *PistonPushLimit::logger;