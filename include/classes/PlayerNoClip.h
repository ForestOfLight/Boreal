#pragma once

#include "../Utils.h"
#include "../PatternScanner.h"
#include "NativePlayerCache.h"

#define NOCLIP_ABILITIES_INDEX 17

class PlayerNoClip {
public:
    struct PlayerNoClipState {
        bool enabled = false;
        bool previousState = false;
    };

    static std::unordered_map<int64_t, PlayerNoClipState> playerStates;
    static endstone::Server *server;

    static void enable(int64_t playerId);
    static void disable(int64_t playerId);
    static void clear(int64_t playerId);
    static bool isEnabled(int64_t playerId);
    static void tick();

private:

    static bool shouldNoClip(const endstone::Player *player);
    static bool setNoClip(endstone::Player *player, bool shouldNoClip);
};
