#pragma once
#include <funchook.h>
#include "../Utils.h"

class PlayersTickLevelChunks {
public:
    static std::vector<int64_t> playerIdsNotLoadingChunks;
    static endstone::Logger *logger;

    static void startLoadingChunks(int64_t playerId);
    static void stopLoadingChunks(int64_t playerId);
    static bool shouldBeLoadingChunks(int64_t playerId);
    static void hook(void *baseAddress, funchook_t *funchook);
};
