#pragma once

#include <funchook.h>

class Tick {
public:
    static double targetTickRate;
    static double storedTickRate;
    static int stepTicks;
    static int sprintTicks;

    static bool isFrozen() {
        return targetTickRate == 0.0 && stepTicks == 0 && sprintTicks == 0;
    }

    static bool isStepping() {
        return stepTicks > 0;
    }

    static bool isSprinting() {
        return sprintTicks > 0;
    }

    static void decrementStepTicks() {
        stepTicks--;
    }

    static void decrementSprintTicks() {
        sprintTicks--;
    }

    static void hook(void *startingAddress, funchook_t *funchook);
};

inline double Tick::targetTickRate = 20.0;
inline double Tick::storedTickRate = 20.0;
inline int Tick::stepTicks = 0;
inline int Tick::sprintTicks = 0;

void (*minecraftTickFn)();
void tickHook();

inline void Tick::hook(void *startingAddress, funchook_t *funchook) {
    int rv;
#ifdef __GNUC__
    void *tickAddr = (char *)startingAddress + 135866944; // address of "_ZN5Level4tickEv"
#else
    void *tickAddr = (char *)startingAddress + 44663120; // address of "?tick@Level@@UEAAXXZ"
#endif
    /* Preparekhooking.
             * The return value is used to call the original tick function
             * in tick_hook.
             */
    minecraftTickFn = (void(*)())tickAddr;

    rv = funchook_prepare(funchook, (void **)&minecraftTickFn, (void*)&tickHook);
    if (rv != 0) {
    }
}

void tickHook() {
    if (Tick::isFrozen()) {
        minecraftTickFn();
    } else if (Tick::isStepping()) {
        minecraftTickFn();
        Tick::decrementStepTicks();
    }// else if (Tick::isSprinting()) {
    //     Tick::decrementSprintTicks();
    // }

    // if (Tick::tickSlowdown != 0) {
    //     Tick::tickCounter = (Tick::tickCounter + 1) % Tick::tickSlowdown;
    // }
}
