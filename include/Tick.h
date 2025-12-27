#pragma once

#include <funchook.h>
#include <endstone/event/player/player_quit_event.h>

void (*minecraftLevelTickFn)(void *level);
void (*minecraftServerInstanceUpdateFn)(void *serverInstance);

class TickSpeed {
public:
    static float targetTickRate;
    static bool isFrozen;
    static endstone::CommandSender *freezeSender;
    static int stepTicks;
    static float sprintTicks;
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
        std::string message = fmt::format("§7Sprint completed at {} tps ({:.1f} mspt).", tps, mspt);
        server->broadcastMessage(message);
    }

    static bool shouldStartSprint(float *realSprintTicks) {
        return *realSprintTicks == -1 && TickSpeed::sprintTicks > 0;
    }

    static bool isDoneSprinting(float *realSprintTicks) {
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
inline float TickSpeed::sprintTicks = 0;
inline bool TickSpeed::shouldInterruptSprint = false;
inline std::chrono::time_point<std::chrono::system_clock> TickSpeed::sprintStartDate;
inline endstone::Server *TickSpeed::server;
inline endstone::Logger *TickSpeed::logger;

void serverInstanceHook(void *serverInstance) {
    auto* serverTimer = *reinterpret_cast<char**>(reinterpret_cast<char*>(serverInstance) + 0xD0);
    auto* clientTimer = *reinterpret_cast<char**>(reinterpret_cast<char*>(serverInstance) + 0xD8);
    auto* serverTickRate = reinterpret_cast<float*>(serverTimer + 0x0);
    auto* clientTickRate = reinterpret_cast<float*>(clientTimer + 0x0);
    *serverTickRate = TickSpeed::targetTickRate;
    *clientTickRate = TickSpeed::targetTickRate;
    if (TickSpeed::isStepping()) {
        *serverTickRate = 20.0;
        *clientTickRate = 20.0;
    } else if (TickSpeed::isFrozen) {
        *serverTickRate = 0.0;
        *clientTickRate = 0.0;
    }

    auto* serverSprintTicks = reinterpret_cast<float*>(serverTimer + 0x3C);
    if (TickSpeed::shouldStartSprint(serverSprintTicks)) {
        *serverSprintTicks = TickSpeed::sprintTicks;
    } else if (TickSpeed::shouldInterruptSprint) {
        TickSpeed::shouldInterruptSprint = false;
        *serverSprintTicks = -1;
    } else if (TickSpeed::isDoneSprinting(serverSprintTicks)) {
        *serverSprintTicks = -1;
        TickSpeed::finishSprint();
    }

    if (*serverSprintTicks == 0) {
        *serverSprintTicks = -1;
    }
    minecraftServerInstanceUpdateFn(serverInstance);
}

void tickHook(void *level) {
    if (TickSpeed::isStepping()) {
        TickSpeed::stepTicks--;
    }
    minecraftLevelTickFn(level);
}

inline void TickSpeed::hook(void *baseAddress, funchook_t *funchook) {
#ifdef __GNUC__
    void *tickAddr = (char *)baseAddress + 162042432; // address of "_ZN5Level4tickEv"
    void *serverInstanceAddr = (char *)baseAddress + 134948960; // address of "ServerInstance::_update(void* serverInstance)"
#else
    void *tickAddr = (char *)baseAddress + 67968608; // address of "?tick@Level@@UEAAXXZ"
    void *serverInstanceAddr = (char *)baseAddress + 50846416; // address of "ServerInstance::_update(void* serverInstance)"
#endif
    minecraftLevelTickFn = (void(*)(void*))tickAddr;
    int errorCode = funchook_prepare(funchook, (void **)&minecraftLevelTickFn, tickHook);
    if (errorCode) {
        logger->error("Failed to prepare hook: {}", funchook_error_message(funchook));
    }
    minecraftServerInstanceUpdateFn = (void(*)(void*))serverInstanceAddr;
    errorCode = funchook_prepare(funchook, (void **)&minecraftServerInstanceUpdateFn, serverInstanceHook);
    if (errorCode) {
        logger->error("Failed to prepare hook: {}", funchook_error_message(funchook));
    }
}
