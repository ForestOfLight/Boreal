#include "classes/PlayerNoClip.h"

#include "classes/PlayerAbilities.h"

std::unordered_map<int64_t, PlayerNoClip::PlayerNoClipState> PlayerNoClip::playerStates = {};
endstone::Server *PlayerNoClip::server = nullptr;

void PlayerNoClip::enable(const int64_t playerId) {
    playerStates[playerId].enabled = true;
}

void PlayerNoClip::disable(const int64_t playerId) {
    playerStates[playerId].enabled = false;
}

void PlayerNoClip::clear(const int64_t playerId) {
    playerStates.erase(playerId);
}

bool PlayerNoClip::isEnabled(const int64_t playerId) {
    auto playerState = playerStates.find(playerId);
    return playerState != playerStates.end() && playerState->second.enabled;
}

void PlayerNoClip::tick() {
    if (!server)
        return;
    for (auto *player : server->getOnlinePlayers()) {
        const bool shouldEnable = shouldNoClip(player);
        setNoClip(player, shouldEnable);
    }
}

bool PlayerNoClip::shouldNoClip(const endstone::Player *player) {
    const int64_t playerId = player->getId();
    return isEnabled(playerId) && player->getGameMode() == endstone::GameMode::Creative && player->isFlying();
}

bool PlayerNoClip::setNoClip(endstone::Player *player, const bool shouldNoClip) {
    auto playerState = playerStates.find(player->getId());
    if (playerState == playerStates.end() || playerState->second.previousState == shouldNoClip)
        return false;
    playerState->second.previousState = shouldNoClip;
    return PlayerAbilities::setAbility(player, NOCLIP_ABILITIES_INDEX, shouldNoClip);
}
