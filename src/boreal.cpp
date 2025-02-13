// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.

#include "boreal.h"

#include <cstdio>
#include <funchook.h>
#include <hook.h>
#include <string>
#include <system_error>
#include <unordered_map>

// The ENDSTONE_PLUGIN macro defines the metadata for the plugin.
ENDSTONE_PLUGIN(/*name=*/"boreal", /*version=*/"0.1.0", /*main_class=*/Boreal)
{
    prefix = "Boreal";
    description = "C++ example plugin for Endstone servers";
    website = "";
    authors = {"R2leyser"};

    command("flyspeed")
        .description("A command to change your fly speed")
        .usages("/flyspeed [speed: float]")
        .aliases("fs")
        .permissions("boreal.command.flyspeed");

    command("freeze")
        .description("Command to freeze the game")
        .usages("/freeze")
        .permissions("boreal.command.op");

    command("tickslowdown")
        .description("Command to freeze the game")
        .usages("/tickslowdown <n: int>")
        .permissions("boreal.command.op");

    permission("boreal.command")
        .description("Allow users to use all commands provided by this example plugin")
        .children("boreal.command.flyspeed", true)
        .children("boreal.command.op", true);

    permission("boreal.command.flyspeed")
        .description("Allow users to use the flyspeed command")
        .default_(endstone::PermissionDefault::Operator);

    permission("boreal.command.op")
        .description("Set the command permission to op only")
        .default_(endstone::PermissionDefault::Operator);
}

void tick_hook()
{
    if (!Tick::tickFreeze && Tick::tickCounter == 0){
        tick_func();
    }
   
    Tick::tickCounter = (Tick::tickCounter + 1) % Tick::tickSlowdown;

    return;
}

int install_hooks(ssize_t startingAddress)
{

    int rv;
    std::printf("entered install_hooks\n");

    funchook_set_debug_file("funchook-debug");
    funchook_t *funchook = funchook_create();

    printf("Created funchook\n");
    printf("startingAddress: %ld\n", startingAddress);

    ssize_t tickAddr = startingAddress + 130259312; // address of "_ZN5Level4tickEv"
    printf("tick addr: %ld\n", tickAddr);
    /* Preparekhooking.
     * The return value is used to call the original tick function
     * in tick_hook.
     */
    tick_func = (void(*)())tickAddr;
    std::printf("Right before funchook_prepare\n");
    funchook_prepare(funchook, (void **)&tick_func, (void*)&tick_hook);
    printf("funchook prepare");
    if (rv != 0) {
    }

    /* Install hooks.
     * The first 5-byte code of tick() and recv() are changed respectively.
     */
    rv = funchook_install(funchook, 0);
    if (rv != 0) {
        /* error */
    }
    return rv;

}
