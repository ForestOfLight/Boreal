#include "classes/PlayerAbilities.h"

#include "PatternScanner.h"
#include "classes/NativePlayerCache.h"

endstone::Logger *PlayerAbilities::logger = nullptr;
uintptr_t PlayerAbilities::layer5Offset = 0;
bool PlayerAbilities::successfulInstall = false;

bool PlayerAbilities::setAbility(endstone::Player *player, uint8_t abilityIndex, bool value) {
    const int64_t playerId = player->getId();
    void *nativePlayer = NativePlayerCache::getByUniqueId(playerId);
    if (!nativePlayer)
        return false;
    auto *abilities = reinterpret_cast<Actor *>(nativePlayer)->getEntity().tryGetComponent<AbilitiesComponent>();
    if (!abilities)
        return false;
    setServerAbility(abilities, abilityIndex, value);
    updateClientAbilities(player);
    return true;
}

void PlayerAbilities::install(void *baseAddress) {
    const auto matches = PatternScanner::scan(baseAddress, kGetAbilityPattern, sizeof(kGetAbilityPattern));
    bool found = false;
    for (const auto &match : matches) {
        if (tryExtractOffsets(match.address)) {
            found = true;
            break;
        }
    }
    successfulInstall = found;
    if (!found)
        logger->warning("Could not locate LayeredAbilities::getAbility; /noclip will be unavailable");
}

bool PlayerAbilities::isInstalled() {
    return successfulInstall;
}

void PlayerAbilities::setServerAbility(AbilitiesComponent *abilities, uint8_t abilityIndex, bool value) {
    char *slot = reinterpret_cast<char *>(abilities) + layer5Offset + 12 * abilityIndex;
    if (*slot == 1) {
        *slot = 2;
        slot[4] = 0;
    }
    slot[4] = value;
}

void PlayerAbilities::updateClientAbilities(const endstone::Player *player) {
    player->setFlySpeed(player->getFlySpeed()); // A hack to send a packet updating the player's abilities.
}

bool PlayerAbilities::tryExtractOffsets(const void *fn) {
    const auto *c = reinterpret_cast<const uint8_t *>(fn);
    if (c[kEndOffsetInsnOffset] != 0x48 || c[kEndOffsetInsnOffset+1] != 0x8D || c[kEndOffsetInsnOffset+2] != 0x88)
        return false;
    if (c[kStrideInsnOffset] != 0x48 || c[kStrideInsnOffset+1] != 0x81 || c[kStrideInsnOffset+2] != 0xC1)
        return false;

    const uint32_t endOffset = *reinterpret_cast<const uint32_t *>(c + kEndOffsetInsnOffset + 3);
    const int32_t  strideNeg = *reinterpret_cast<const int32_t *> (c + kStrideInsnOffset    + 3);
    if (strideNeg >= 0)
        return false;

    layer5Offset = endOffset - static_cast<uintptr_t>(-strideNeg);
    return true;
}
