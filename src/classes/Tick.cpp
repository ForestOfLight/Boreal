#include "classes/Tick.h"
#include "classes/PlayerNoClip.h"
#include <fmt/format.h>
#include <chrono>

float TickSpeed::targetTickRate = 20.0f;
bool TickSpeed::isFrozen = false;
endstone::CommandSender *TickSpeed::freezeSender = nullptr;
int TickSpeed::stepTicks = 0;
float TickSpeed::sprintTicks = 0.0f;
bool TickSpeed::shouldInterruptSprint = false;
std::chrono::time_point<std::chrono::system_clock> TickSpeed::sprintStartDate;
endstone::Server *TickSpeed::server = nullptr;
endstone::Logger *TickSpeed::logger = nullptr;

// Internal trampoline pointers — not exposed in the header.
static void (*minecraftLevelTickFn)(void *level) = nullptr;
static void (*minecraftServerInstanceUpdateFn)(void *serverInstance) = nullptr;

static void serverInstanceHook(void *serverInstance) {
    auto *serverTimer = *reinterpret_cast<char **>(reinterpret_cast<char *>(serverInstance) + 0xD0);
    auto *clientTimer = *reinterpret_cast<char **>(reinterpret_cast<char *>(serverInstance) + 0xD8);
    auto *serverTickRate = reinterpret_cast<float *>(serverTimer + 0x0);
    auto *clientTickRate = reinterpret_cast<float *>(clientTimer + 0x0);
    *serverTickRate = TickSpeed::targetTickRate;
    *clientTickRate = TickSpeed::targetTickRate;
    if (TickSpeed::isStepping()) {
        *serverTickRate = 20.0f;
        *clientTickRate = 20.0f;
    } else if (TickSpeed::isFrozen) {
        *serverTickRate = 0.0f;
        *clientTickRate = 0.0f;
    }

    auto *serverSprintTicks = reinterpret_cast<float *>(serverTimer + 0x3C);
    if (TickSpeed::shouldStartSprint(serverSprintTicks)) {
        *serverSprintTicks = TickSpeed::sprintTicks;
    } else if (TickSpeed::shouldInterruptSprint) {
        TickSpeed::shouldInterruptSprint = false;
        *serverSprintTicks = -1;
    } else if (TickSpeed::isDoneSprinting(serverSprintTicks)) {
        *serverSprintTicks = -1;
        TickSpeed::finishSprint();
    }

    if (*serverSprintTicks == 0)
        *serverSprintTicks = -1;

    minecraftServerInstanceUpdateFn(serverInstance);
}

static void tickHook(void *level) {
    if (TickSpeed::isStepping())
        TickSpeed::stepTicks--;
    minecraftLevelTickFn(level);
    PlayerNoClip::tick();
}

void TickSpeed::setRate(float rate) {
    unfreeze();
    targetTickRate = rate;
    if (targetTickRate <= 0)
        targetTickRate = 0.0f;
}

void TickSpeed::freeze(endstone::CommandSender &sender) {
    freezeSender = &sender;
    isFrozen = true;
}

void TickSpeed::unfreeze() {
    freezeSender = nullptr;
    isFrozen = false;
    if (targetTickRate == 0.0f)
        targetTickRate = 20.0f;
}

void TickSpeed::step(int ticks) {
    stepTicks += ticks;
}

void TickSpeed::sprint(endstone::CommandSender &sender, int ticks) {
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
    sprintTicks = static_cast<float>(ticks);
    server->broadcastMessage("§7Sprinting {} ticks...", ticks);
}

void TickSpeed::finishSprint() {
    int completedTicks = static_cast<int>(sprintTicks);
    double msToCompletion = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now() - sprintStartDate).count();
    if (msToCompletion == 0.0)
        msToCompletion = 1.0;
    int tps = static_cast<int>(1000.0 * completedTicks / msToCompletion);
    double mspt = msToCompletion / completedTicks;
    sprintTicks = 0;
    std::string message = fmt::format("§7Sprint completed at {} tps ({:.1f} mspt).", tps, mspt);
    server->broadcastMessage(message);
}

bool TickSpeed::shouldStartSprint(float *realSprintTicks) {
    return *realSprintTicks == -1 && TickSpeed::sprintTicks > 0;
}

bool TickSpeed::isDoneSprinting(float *realSprintTicks) {
    return *realSprintTicks == 0 && TickSpeed::sprintTicks > 0;
}

bool TickSpeed::isSprinting() {
    return sprintTicks > 0;
}

bool TickSpeed::isStepping() {
    return stepTicks > 0;
}

void TickSpeed::onPlayerQuit(endstone::PlayerQuitEvent &event) {
    if (freezeSender != nullptr && event.getPlayer().getName() == freezeSender->getName()) {
        unfreeze();
        server->broadcastMessage("§7Freezing player quit. The game is running normally.");
    }
}

void TickSpeed::hook(void *baseAddress, funchook_t *funchook) {
#ifdef __GNUC__
    void *tickAddr = (char *)baseAddress + ?; // address of "_ZN5Level4tickEv"
    void *serverInstanceAddr = (char *)baseAddress + ?; // address of "ServerInstance::_update"
#else
    void *tickAddr = (char *)baseAddress + 65184224; // address of "?tick@Level@@UEAAXXZ"
    void *serverInstanceAddr = (char *)baseAddress + 43253872; // address of "ServerInstance::_update"
#endif
    minecraftLevelTickFn = (void(*)(void*))tickAddr;
    int errorCode = funchook_prepare(funchook, (void **)&minecraftLevelTickFn, tickHook);
    if (errorCode)
        logger->error("Failed to prepare hook: {}", funchook_error_message(funchook));

    minecraftServerInstanceUpdateFn = (void(*)(void*))serverInstanceAddr;
    errorCode = funchook_prepare(funchook, (void **)&minecraftServerInstanceUpdateFn, serverInstanceHook);
    if (errorCode)
        logger->error("Failed to prepare hook: {}", funchook_error_message(funchook));
}

