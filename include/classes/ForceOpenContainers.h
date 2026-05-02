#pragma once
#include <funchook.h>
#include "../Utils.h"

class ForceOpenContainers {
public:
    static endstone::Logger *logger;
    static bool isEnabled;

    static void enable();
    static void disable();
    static void hook(void *baseAddress, funchook_t *funchook);
};
