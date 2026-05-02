#pragma once
#include <endstone/logger.h>
#include "../MemoryPatch.h"
#include "../PatternScanner.h"
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

    static void scanFunction(void *fnStart, size_t searchSize);

public:
    static endstone::Logger *logger;

    static void setPistonPushLimit(int newLimit);
    static int getPistonPushLimit();
    static void install(void *baseAddress);
    static void uninstall();
};
