#pragma once

#include "endstone/level/level.h"
#include <cstdio>

#include <string>
#include <fstream>

#if defined(__GNUC__) 
    size_t getAddr(){
        std::string addressRange;
        std::ifstream mapsFile("/proc/self/maps");

        if (mapsFile.is_open()) {
            std::string line;
            std::getline(mapsFile, line);

            size_t start = line.find("-");
            size_t end = line.find(" ");

            addressRange = line.substr(0, start);

            mapsFile.close();
        } else {
            std::printf("Failed to open /proc/self/maps");
        }
        return (ssize_t)std::stol(addressRange, NULL, 16);
    }
#endif 
int install_hooks(ssize_t startingAddress);
static void (*original_tick_func)();
static void tick_hook();

namespace Tick {
    static bool tickFreeze = false;
    static int tickCounter = 0;
    static int tickSlowdown = 1;
    static int tickAccel = 1;
    static int stepCounter = 0;
}
