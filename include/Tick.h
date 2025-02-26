#pragma once

#include <funchook.h>

void (*minecraftLevelTickFn)();
void (*minecraftAdvanceTicksFn)(void *timer, float advance);

class Tick {
public:
    static float targetTickRate;
    static long mspt; // this value could probably be removed.
    static bool isFrozen;
    static int stepTicks;

    static int sprintTicks;
    static endstone::CommandSender *sprintSender;
    static std::chrono::time_point<std::chrono::system_clock> sprintStartDate;

    static endstone::Logger *logger;

    static void setRate(float rate) {
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
        if (sprintTicks > 0) {
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

    static void hook(void *baseAddress, funchook_t *funchook);
};

void levelTickHook() {
    // if (Tick::stepTicks > 0) {
    //     Tick::stepTicks--;
    // } else if (Tick::isFrozen) {
    //     return;
    // }
    minecraftLevelTickFn();
}

void timerHook(void *timer, float advance) {
    *(float *)((char *)timer + 0x0) = Tick::targetTickRate;
    if (Tick::stepTicks > 0) {
        Tick::stepTicks--;
    } else if (Tick::isFrozen) {
        *(float *)((char *)timer + 0x0) = 0.0;
    }

    int32_t currentSprintTicks = *(int32_t *)((char *)timer + 0x3C);
    if (currentSprintTicks == -1 && Tick::sprintTicks > 0) {
        *(int32_t *)((char *)timer + 0x3C) = Tick::sprintTicks;
        Tick::sprintSender->sendMessage("Sprint started.");
    } else if (currentSprintTicks == 0) {
        *(int32_t *)((char *)timer + 0x3C) = -1;
        Tick::sprintTicks = 0;
    }
    minecraftAdvanceTicksFn(timer, advance);
}

inline float Tick::targetTickRate = 20.0;
inline long Tick::mspt = 50;
inline bool Tick::isFrozen = false;
inline int Tick::stepTicks = 0;
inline int Tick::sprintTicks = 0;
inline endstone::CommandSender *Tick::sprintSender = nullptr;
inline std::chrono::time_point<std::chrono::system_clock> Tick::sprintStartDate;
inline endstone::Logger *Tick::logger;

inline void Tick::hook(void *baseAddress, funchook_t *funchook) {
    int rv;
#ifdef __GNUC__
    void *tickAddr = (char *)baseAddress + 135866944; // address of "_ZN5Level4tickEv"
#else
    void *tickAddr = (char *)baseAddress + 44663120; // address of "?tick@Level@@UEAAXXZ"
    void *tpsAddr = (char *)baseAddress + 40916688; // address of "?advanceTime@Timer@@QEAAXM@Z"
#endif
    minecraftLevelTickFn = (void(*)())tickAddr;
    minecraftAdvanceTicksFn = (void(*)(void*, float))tpsAddr;

    rv = funchook_prepare(funchook, (void **)&minecraftLevelTickFn, levelTickHook);
    if (rv != 0) {
    }
    rv = funchook_prepare(funchook, (void **)&minecraftAdvanceTicksFn, timerHook);
    if (rv != 0) {
    }
}
