#pragma once
#include <endstone/game_mode.h>
#include "../Utils.h"
#include "../PatternScanner.h"
#include "NativePlayerCache.h"

#define ABILITIES_INDEX_NOCLIP 17

class PlayerNoClip {
public:
    struct PlayerNoClipState {
        bool enabled = false;
        bool previousState = false;
    };

    static std::unordered_map<int64_t, PlayerNoClipState> playerStates;
    static endstone::Server *server;
    static endstone::Logger *logger;
    static uintptr_t layer5Offset;
    static bool ready;

    static void enable(const int64_t playerId) {
        playerStates[playerId].enabled = true;
    }

    static void disable(const int64_t playerId) {
        auto playerState = playerStates.find(playerId);
        if (playerState != playerStates.end()) {
            playerState->second.enabled = false;
            endstone::Player *player = Utils::getPlayerFromUniqueID(server, playerId);
            setNoClip(player, false);
        }
    }

    static void clearState(const int64_t playerId) {
        playerStates.erase(playerId);
    }

    static bool isEnabled(const int64_t playerId) {
        auto it = playerStates.find(playerId);
        return it != playerStates.end() && it->second.enabled;
    }

    static void tick() {
        if (!server)
            return;
        for (auto *player : server->getOnlinePlayers()) {
            const bool shouldEnable = shouldNoClip(player);
            setNoClip(player, shouldEnable);
        }
    }

    static void install(void *baseAddress);

private:
    static constexpr uint8_t kGetAbilityPattern[] = {
        0x48, 0x83, 0xEC, 0x38,  // sub rsp, 38h
        0x8B, 0x41, 0x18,        // mov eax, [rcx+18h]
        0x48, 0x8B, 0x49, 0x10,  // mov rcx, [rcx+10h]
        0x4C, 0x0F, 0xBE, 0xDA,  // movsx r11, dl
    };
    // Offsets within the matched function where the necessary constants live:
    //   fn+0x22: 48 8D 88 [END_OFFSET:u32]  — lea rcx, [rax + end_offset]
    //   fn+0x40: 48 81 C1 [NEG_STRIDE:i32]  — add rcx, -stride
    static constexpr size_t kEndOffsetInsnOff = 0x22;
    static constexpr size_t kStrideInsnOff    = 0x40;

    static bool setNoClip(endstone::Player *player, const bool shouldNoClip) {
        auto playerState = playerStates.find(player->getId());
        if (playerState == playerStates.end() || !playerState->second.enabled)
            return false;

        const int64_t playerId = player->getId();
        void *nativePlayer = NativePlayerCache::getByUniqueId(playerId);
        if (!nativePlayer)
            return false;
        auto *abilities = reinterpret_cast<Actor *>(nativePlayer)->getEntity().tryGetComponent<AbilitiesComponent>();
        if (!abilities)
            return false;
        setAbility(abilities, ABILITIES_INDEX_NOCLIP, shouldNoClip);

        if (playerState->second.previousState != shouldNoClip) {
            playerState->second.previousState = shouldNoClip;
            updateClientAbilities(player);
        }
        return true;
    }

    static bool shouldNoClip(const endstone::Player *player) {
        const int64_t playerId = player->getId();
        return isEnabled(playerId) && player->getGameMode() == endstone::GameMode::Creative && player->isFlying();
    }

    static void updateClientAbilities(const endstone::Player *player) {
        player->setFlySpeed(player->getFlySpeed()); // A hack to send a packet updating the player's abilities.
    }

    static bool tryExtractOffsets(const void *fn) {
        const auto *c = reinterpret_cast<const uint8_t *>(fn);
        if (c[kEndOffsetInsnOff] != 0x48 || c[kEndOffsetInsnOff+1] != 0x8D || c[kEndOffsetInsnOff+2] != 0x88)
            return false;
        if (c[kStrideInsnOff] != 0x48 || c[kStrideInsnOff+1] != 0x81 || c[kStrideInsnOff+2] != 0xC1)
            return false;

        const uint32_t endOffset = *reinterpret_cast<const uint32_t *>(c + kEndOffsetInsnOff + 3);
        const int32_t  strideNeg = *reinterpret_cast<const int32_t *> (c + kStrideInsnOff    + 3);
        if (strideNeg >= 0)
            return false;

        layer5Offset = endOffset - static_cast<uintptr_t>(-strideNeg);
        return true;
    }

    static void setAbility(AbilitiesComponent *abilities, uint8_t index, bool value) {
        char *slot = reinterpret_cast<char *>(abilities) + layer5Offset + 12 * index;
        if (*slot == 1) {
            *slot = 2;
            slot[4] = 0;
        }
        slot[4] = value;
    }
};

inline std::unordered_map<int64_t, PlayerNoClip::PlayerNoClipState> PlayerNoClip::playerStates = {};
inline endstone::Server *PlayerNoClip::server;
inline endstone::Logger *PlayerNoClip::logger;
inline uintptr_t PlayerNoClip::layer5Offset = 0x4C8;
inline bool PlayerNoClip::ready = false;

inline void PlayerNoClip::install(void *baseAddress) {
    // Search for LayeredAbilities::getAbility
    const auto matches = PatternScanner::scan(baseAddress, kGetAbilityPattern, sizeof(kGetAbilityPattern));
    bool found = false;
    for (const auto &match : matches) {
        if (tryExtractOffsets(match.address)) {
            found = true;
            break;
        }
    }
    ready = found;
    if (!found)
        logger->warning("Could not locate LayeredAbilities::getAbility; /noclip will be unavailable");
}