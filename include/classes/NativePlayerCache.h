#pragma once
#include <unordered_map>
#include <cstdint>

namespace endstone { class Player; }

class NativePlayerCache {
public:
    static std::unordered_map<int64_t, void *> playerPtrs;

    static void cache(int64_t id, void *ptr);
    static void remove(int64_t id);
    static void *getByUniqueId(int64_t id);
    static void *getByEndstonePlayer(const endstone::Player *player);
};
