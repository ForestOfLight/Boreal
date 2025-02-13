#pragma once

#include <cstdio>

int install_hooks(ssize_t startingAddress);
static void (*tick_func)();
static void tick_hook();

namespace Tick {
    static bool tickFreeze = false;
    static int tickCounter = 0;
    static int tickSlowdown = 1;
}
