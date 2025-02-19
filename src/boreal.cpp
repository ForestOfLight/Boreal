// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.

#include "boreal.h"
#include "endstone/level/level.h"

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


    command("listactors")
        .description("list all the actors loaded")
        .usages("/listactors")
        .permissions("boreal.command.op");

    command("flyspeed")
        .description("A command to change your fly speed")
        .usages("/flyspeed [speed: float]")
        .aliases("fs")
        .permissions("boreal.command.flyspeed");
        .description("Command to change the tick speed of the game")
        .usages("/tick (sprint)<a: bool> <n: int>")
        .usages("/tick (slow)<a: bool> <n: int>")
        .usages("/tick (freeze) <b: bool>")
        .usages("/tick (unfreeze) <b: bool>")
        .usages("/tick (step) <b: bool> [n: int]")
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
        original_tick_func();
        for(int i = 0; i < 1; i++){
            original_tick_func();
        }
    } else if (Tick::tickFreeze && Tick::stepCounter != 0){
        original_tick_func();
        Tick::stepCounter--;
    }

    if (Tick::tickSlowdown != 0) {Tick::tickCounter = (Tick::tickCounter + 1) % Tick::tickSlowdown;}

    return;
}

int install_hooks(void * startingAddress)
{

    int rv;

    funchook_set_debug_file("funchook-debug");
    funchook_t *funchook = funchook_create();

#ifdef __GNUC__
    void *tickAddr = (char *)startingAddress + 130259312; // address of "_ZN5Level4tickEv"
#else 
    void *tickAddr = (char *)startingAddress + 39489328; // address of "?tick@Level@@UEAAXXZ"
#endif
    /* Preparekhooking.
     * The return value is used to call the original tick function
     * in tick_hook.
     */
    original_tick_func = (void(*)())tickAddr;

    rv = funchook_prepare(funchook, (void **)&original_tick_func, (void*)&tick_hook);
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
