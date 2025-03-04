#pragma once

#include <funchook.h>
#include <endstone/event/player/player_quit_event.h>

void (*minecraftAdvanceTicksFn)(void *timer, float advance);
void (*minecraftLevelTickFn)(void *level);

class TickSpeed {
public:
    static float targetTickRate;
    static bool isFrozen;
    static endstone::CommandSender *freezeSender;
    static int stepTicks;
    static int sprintTicks;
    static bool shouldInterruptSprint;
    static std::chrono::time_point<std::chrono::system_clock> sprintStartDate;

    static endstone::Server *server;
    static endstone::Logger *logger;

    static void setRate(float rate) {
        unfreeze();
        targetTickRate = rate;
        if (targetTickRate <= 0) {
            targetTickRate = 0.0;
        }
    }

    static void freeze(endstone::CommandSender &sender) {
        freezeSender = &sender;
        isFrozen = true;
    }

    static void unfreeze() {
        freezeSender = nullptr;
        isFrozen = false;
        if (targetTickRate == 0.0) {
            targetTickRate = 20.0;
        }
    }

    static void step(int ticks) {
        stepTicks += ticks;
    }

    static void sprint(endstone::CommandSender &sender, int ticks) {
        if (ticks == 0) {
            shouldInterruptSprint = true;
            if (sprintTicks > 0) {
                finishSprint();
                server->broadcastMessage("§7Interrupted current tick sprint.");
            }
            return;
        }
        if (sprintTicks > 0) {
            server->broadcastMessage("§cThe game is already sprinting.");
            return;
        }
        sprintStartDate = std::chrono::system_clock::now();
        sprintTicks = ticks;
        server->broadcastMessage("§7Sprinting {} ticks...", ticks);
    }

    static void finishSprint() {
        int completedTicks = sprintTicks;
        double msToCompletion = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - sprintStartDate).count();
        if (msToCompletion == 0.0) {
            msToCompletion = 1.0;
        }
        int tps = static_cast<int>(1000.0 * completedTicks / msToCompletion);
        double mspt = (1.0*msToCompletion) / completedTicks;
        sprintTicks = 0;
        std::string message = fmt::format("§7Sprint completed at {} tps ({} mspt).", tps, mspt);
        server->broadcastMessage(message);
    }

    static bool shouldStartSprint(int32_t *realSprintTicks) {
        return *realSprintTicks == -1 && TickSpeed::sprintTicks > 0;
    }

    static bool isDoneSprinting(int32_t *realSprintTicks) {
        return *realSprintTicks == 0 && TickSpeed::sprintTicks > 0;
    }

    static bool isSprinting() {
        return sprintTicks > 0;
    }

    static bool isStepping() {
        return stepTicks > 0;
    }

    static void onPlayerQuit(endstone::PlayerQuitEvent &event) {
        if (freezeSender != nullptr && event.getPlayer().getName() == freezeSender->getName()) {
            unfreeze();
            server->broadcastMessage("§7Freezing player quit. The game is running normally.");
        }
    }

    static void hook(void *baseAddress, funchook_t *funchook);
};

inline float TickSpeed::targetTickRate = 20.0;
inline bool TickSpeed::isFrozen = false;
inline endstone::CommandSender *TickSpeed::freezeSender = nullptr;
inline int TickSpeed::stepTicks = 0;
inline int TickSpeed::sprintTicks = 0;
inline bool TickSpeed::shouldInterruptSprint = false;
inline std::chrono::time_point<std::chrono::system_clock> TickSpeed::sprintStartDate;
inline endstone::Server *TickSpeed::server;
inline endstone::Logger *TickSpeed::logger;

void timerHook(void *timer, float advance) {
    auto *realTickRate = reinterpret_cast<float *>(reinterpret_cast<char *>(timer) + 0x0);
    *realTickRate = TickSpeed::targetTickRate;
    if (TickSpeed::isStepping()) {
        *realTickRate = 20.0;
    } else if (TickSpeed::isFrozen) {
        *realTickRate = 0.0;
    }

    auto *realSprintTicks = reinterpret_cast<int32_t *>(reinterpret_cast<char *>(timer) + 0x3C);
    if (TickSpeed::shouldStartSprint(realSprintTicks)) {
        *realSprintTicks = TickSpeed::sprintTicks;
    } else if (TickSpeed::shouldInterruptSprint) {
        TickSpeed::shouldInterruptSprint = false;
        *realSprintTicks = -1;
    } else if (TickSpeed::isDoneSprinting(realSprintTicks)) {
        *realSprintTicks = -1;
        TickSpeed::finishSprint();
    }

    if (*realSprintTicks == 0) {
        *realSprintTicks = -1;
    }
    minecraftAdvanceTicksFn(timer, advance);
}

void tickHook(void *level) {
    if (TickSpeed::isStepping()) {
        TickSpeed::stepTicks--;
    }
    minecraftLevelTickFn(level);
}

inline void TickSpeed::hook(void *baseAddress, funchook_t *funchook) {
#ifdef __GNUC__
    void *tickAddr = (char *)baseAddress + 135866944; // address of "_ZN5Level4tickEv"
#else
    void *tpsAddr = (char *)baseAddress + 40916688; // address of "?advanceTime@Timer@@QEAAXM@Z"
    void *tickAddr = (char *)baseAddress + 44663120; // address of "?tick@Level@@UEAAXXZ"
#endif
    minecraftAdvanceTicksFn = (void(*)(void*, float))tpsAddr;
    int errorCode = funchook_prepare(funchook, (void **)&minecraftAdvanceTicksFn, timerHook);
    if (errorCode) {
        logger->error("Failed to prepare hook: {}", funchook_error_message(funchook));
    }
    minecraftLevelTickFn = (void(*)(void*))tickAddr;
    errorCode = funchook_prepare(funchook, (void **)&minecraftLevelTickFn, tickHook);
    if (errorCode) {
        logger->error("Failed to prepare hook: {}", funchook_error_message(funchook));
    }
}
