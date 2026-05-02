#include "classes/NativePlayerCache.h"
#include <endstone/player.h>

std::unordered_map<int64_t, void *> NativePlayerCache::playerPtrs = {};

void NativePlayerCache::cache(const int64_t id, void *ptr) {
    playerPtrs[id] = ptr;
}

void NativePlayerCache::remove(const int64_t id) {
    playerPtrs.erase(id);
}

void *NativePlayerCache::getByUniqueId(const int64_t id) {
    const auto it = playerPtrs.find(id);
    return it != playerPtrs.end() ? it->second : nullptr;
}

void *NativePlayerCache::getByEndstonePlayer(const endstone::Player *player) {
    return getByUniqueId(player->getId());
}

