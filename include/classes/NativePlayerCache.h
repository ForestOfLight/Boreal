#pragma once
#include <unordered_map>
#include <cstdint>

class NativePlayerCache {
public:
    static std::unordered_map<int64_t, void *> playerPtrs;

    static void cache(const int64_t id, void *ptr) {
        playerPtrs[id] = ptr;
    }

    static void remove(const int64_t id) {
        playerPtrs.erase(id);
    }

    static void *getByUniqueId(const int64_t id) {
        const auto it = playerPtrs.find(id);
        return it != playerPtrs.end() ? it->second : nullptr;
    }

    static void *getByEndstonePlayer(const endstone::Player *player) {
        return getByUniqueId(player->getId());
    }
};

inline std::unordered_map<int64_t, void *> NativePlayerCache::playerPtrs = {};
