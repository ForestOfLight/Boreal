#pragma once

void (*_determineLevelChunksToTick)(void *actor, void *blockSource, void *loadedChunksComponent,
    void *chunksToTickVector, void *chunksToTickBlockEntitiesVector, void *currentTick);
void (*getActorUniqueIDComponent)(void *actor, void *component);

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
    int64_t actorUniqueIDComponentPtr = 0LL;
    getActorUniqueIDComponent(actor, &actorUniqueIDComponentPtr);
    int64_t actorUniqueID = *reinterpret_cast<int64_t *>(actorUniqueIDComponentPtr);
    if (!PlayersTickLevelChunks::shouldBeLoadingChunks(actorUniqueID))
        reinterpret_cast<std::vector<void *> *>(chunksToTickVector)->clear();
}

void getActorUniqueIDComponentHook(void *actor, void *component) {
    getActorUniqueIDComponent(actor, component);
}


inline void PlayersTickLevelChunks::hook(void *baseAddress, funchook_t *funchook) {
#ifdef __GNUC__
    void *_determineLevelChunksToTickAddr = (char *)baseAddress + ?; // address of "?"
#else
    void *_determineLevelChunksToTickAddr = (char *)baseAddress + 24077088; // address of "LevelChunkTickingSystem::_determineLevelChunksToTick"
    void *getActorUniqueIDComponentAddr = (char *)baseAddress + 57003088; // address of "Actor::getPersistentComponent<ActorUniqueIDComponent>"
#endif
    _determineLevelChunksToTick = (void(*)(void*, void*, void*, void*, void*, void*))_determineLevelChunksToTickAddr;
    int errorCode = funchook_prepare(funchook, (void **)&_determineLevelChunksToTick, tickLevelChunksAroundActorHook);
    if (errorCode) {
        logger->error("Failed to prepare hook: {}", funchook_error_message(funchook));
    }
    getActorUniqueIDComponent = (void(*)(void*, void*))getActorUniqueIDComponentAddr;
    errorCode = funchook_prepare(funchook, (void **)&getActorUniqueIDComponent, getActorUniqueIDComponentHook);
    if (errorCode) {
        logger->error("Failed to prepare hook: {}", funchook_error_message(funchook));
    }
}

