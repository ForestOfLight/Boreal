#pragma once

void (*_determineLevelChunksToTick)(void *actor, void *blockSource, void *loadedChunksComponent,
    void *chunksToTickVector, void *chunksToTickBlockEntitiesVector, void *currentTick);

class PlayersTickLevelChunks {
public:
    static std::vector<int64_t> playerIdsNotLoadingChunks;

    static endstone::Logger *logger;

    static void startLoadingChunks(const int64_t playerId) {
        std::erase(playerIdsNotLoadingChunks, playerId);
    }

    static void stopLoadingChunks(const int64_t playerId) {
        std::erase(playerIdsNotLoadingChunks, playerId);
        playerIdsNotLoadingChunks.push_back(playerId);
    }

    static bool shouldBeLoadingChunks(const int64_t playerId) {
        return std::find(playerIdsNotLoadingChunks.begin(),
                         playerIdsNotLoadingChunks.end(),
                         playerId) == playerIdsNotLoadingChunks.end();
    }

    static void hook(void *baseAddress, funchook_t *funchook);
};

inline std::vector<int64_t> PlayersTickLevelChunks::playerIdsNotLoadingChunks = {};
inline endstone::Logger *PlayersTickLevelChunks::logger;

void tickLevelChunksAroundActorHook(void *actor, void *blockSource, void *loadedChunksComponent,
    void *chunksToTickVector, void *chunksToTickBlockEntitiesVector, void *currentTick) {
    _determineLevelChunksToTick(actor, blockSource, loadedChunksComponent,
        chunksToTickVector, chunksToTickBlockEntitiesVector, currentTick);
    // int64_t actorId = *reinterpret_cast<int64_t *>(reinterpret_cast<char *>(actor) + 0x10);
    // if (!PlayersTickLevelChunks::shouldBeLoadingChunks(actorId))
        reinterpret_cast<std::vector<void *> *>(chunksToTickVector)->clear();
}

inline void PlayersTickLevelChunks::hook(void *baseAddress, funchook_t *funchook) {
#ifdef __GNUC__
    void *tickLevelChunksAroundActorAddr = (char *)baseAddress + ?; // address of "?"
#else
    void *actorLevelChunksToTickAddr = (char *)baseAddress + 24077088; // address of "LevelChunkTickingSystem::_determineLevelChunksToTick"
#endif
    _determineLevelChunksToTick = (void(*)(void*, void*, void*, void*, void*, void*))actorLevelChunksToTickAddr;
    int errorCode = funchook_prepare(funchook, (void **)&_determineLevelChunksToTick, tickLevelChunksAroundActorHook);
    if (errorCode) {
        logger->error("Failed to prepare hook: {}", funchook_error_message(funchook));
    }
}

