#pragma once

#include <funchook.h>

void (*minecraftTickFn)();
void tickHook();

class Tick {
public:
    static double targetTickRate;
    static long mspt; // this value could be used to do tick rate.
    static bool isFrozen;
    static int stepTicks;

    static int sprintTicks;
    static int sprintTicksRemaining;
    static endstone::CommandSender *sprintSender;
    static std::chrono::time_point<std::chrono::system_clock> sprintStartDate;

    static void setRate(double rate) {
        targetTickRate = rate;
        mspt = static_cast<long>(1000.0 / targetTickRate);
        if (mspt <= 0) {
            mspt = 11;
            targetTickRate = 1000.0;
        }
    }

    static void freeze() {
        isFrozen = true;
    }

    static void unfreeze() {
        isFrozen = false;
    }

    static void step(int ticks) {
        if (ticks == 0) {
            ticks = 1;
        }
        stepTicks += ticks;
    }

    static void sprint(endstone::CommandSender &sender, int ticks) {
        if (ticks == 0) {
            finishSprint();
            sender.sendMessage("Sprint interrupted.");
        }
        if (sprintTicksRemaining > 0) {
            sender.sendMessage("Tick speed is already sprinting.");
        }
        sprintSender = &sender;
        sprintStartDate = std::chrono::system_clock::now();
        sprintTicks = ticks;
        sender.sendMessage("Sprinting {} ticks...", ticks);
    }

    static void finishSprint() {
        sprintTicks = 0;
    }

    static void hook(void *startingAddress, funchook_t *funchook);
};

inline double Tick::targetTickRate = 20.0;
inline long Tick::mspt = 50;
inline bool Tick::isFrozen = false;
inline int Tick::stepTicks = 0;
inline int Tick::sprintTicks = 0;
inline int Tick::sprintTicksRemaining = 0;
inline endstone::CommandSender *Tick::sprintSender = nullptr;
inline std::chrono::time_point<std::chrono::system_clock> Tick::sprintStartDate;

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
    if (Tick::stepTicks > 0) {
        Tick::stepTicks--;
    } else if (Tick::isFrozen) {
        return;
    }
    minecraftTickFn();
}
