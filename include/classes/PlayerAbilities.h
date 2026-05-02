#pragma once
#include <cstdint>

#include "EnTT.h"
#include "endstone/logger.h"
#include "endstone/player.h"

class PlayerAbilities {
public:
    static endstone::Logger *logger;

    static bool setAbility(endstone::Player *player, uint8_t abilityIndex, bool value);
    static void install(void *baseAddress);
    static bool isInstalled();

private:
    static constexpr uint8_t kGetAbilityPattern[] = {
        0x48, 0x83, 0xEC, 0x38,
        0x8B, 0x41, 0x18,
        0x48, 0x8B, 0x49, 0x10,
        0x4C, 0x0F, 0xBE, 0xDA,
    };
    static constexpr size_t kEndOffsetInsnOffset = 0x22;
    static constexpr size_t kStrideInsnOffset = 0x40;
    static uintptr_t layer5Offset;
    static bool successfulInstall;

    static void setServerAbility(AbilitiesComponent *abilities, uint8_t abilityIndex, bool value);
    static bool tryExtractOffsets(const void *fn);
    static void updateClientAbilities(const endstone::Player *player);
};
