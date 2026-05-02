#pragma once

#include <chrono>
#include <funchook.h>
#include <endstone/server.h>
#include <endstone/command/command_sender.h>
#include <endstone/event/player/player_quit_event.h>

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

    static void setRate(float rate);
    static void freeze(endstone::CommandSender &sender);
    static void unfreeze();
    static void step(int ticks);
    static void sprint(endstone::CommandSender &sender, int ticks);
    static void finishSprint();
    static bool shouldStartSprint(float *realSprintTicks);
    static bool isDoneSprinting(float *realSprintTicks);
    static bool isSprinting();
    static bool isStepping();
    static void onPlayerQuit(endstone::PlayerQuitEvent &event);
    static void hook(void *baseAddress, funchook_t *funchook);
};
