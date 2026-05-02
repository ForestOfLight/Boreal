#include "classes/PlayersTickLevelChunks.h"
#include "classes/NativePlayerCache.h"

std::vector<int64_t> PlayersTickLevelChunks::playerIdsNotLoadingChunks = {};
endstone::Logger *PlayersTickLevelChunks::logger = nullptr;

// Internal trampoline pointer — not exposed in the header.
static void (*_determineLevelChunksToTick)(void *actor, void *blockSource, void *loadedChunksComponent,
                                            void *chunksToTickVector, void *chunksToTickBlockEntitiesVector,
                                            void *currentTick) = nullptr;

static void tickLevelChunksAroundActorHook(void *actor, void *blockSource, void *loadedChunksComponent,
                                            void *chunksToTickVector, void *chunksToTickBlockEntitiesVector,
                                            void *currentTick) {
    _determineLevelChunksToTick(actor, blockSource, loadedChunksComponent,
        chunksToTickVector, chunksToTickBlockEntitiesVector, currentTick);
    int64_t actorUniqueID = Utils::getUniqueIDFromActorPtr(actor);
    if (!PlayersTickLevelChunks::shouldBeLoadingChunks(actorUniqueID))
        reinterpret_cast<std::vector<void *> *>(chunksToTickVector)->clear();
    NativePlayerCache::cache(actorUniqueID, actor);
}

void PlayersTickLevelChunks::startLoadingChunks(const int64_t playerId) {
    std::erase(playerIdsNotLoadingChunks, playerId);
}

void PlayersTickLevelChunks::stopLoadingChunks(const int64_t playerId) {
    std::erase(playerIdsNotLoadingChunks, playerId);
    playerIdsNotLoadingChunks.push_back(playerId);
}

bool PlayersTickLevelChunks::shouldBeLoadingChunks(const int64_t playerId) {
    return std::find(playerIdsNotLoadingChunks.begin(),
                     playerIdsNotLoadingChunks.end(),
                     playerId) == playerIdsNotLoadingChunks.end();
}

void PlayersTickLevelChunks::hook(void *baseAddress, funchook_t *funchook) {
#ifdef __GNUC__
    void *_determineLevelChunksToTickAddr = (char *)baseAddress + ?; // address of "?"
#else
    void *_determineLevelChunksToTickAddr = (char *)baseAddress + 16550464; // address of "LevelChunkTickingSystem::_determineLevelChunksToTick"
#endif
    _determineLevelChunksToTick = (void(*)(void*, void*, void*, void*, void*, void*))_determineLevelChunksToTickAddr;
    int errorCode = funchook_prepare(funchook, (void **)&_determineLevelChunksToTick, tickLevelChunksAroundActorHook);
    if (errorCode)
        logger->error("Failed to prepare hook: {}", funchook_error_message(funchook));
}

