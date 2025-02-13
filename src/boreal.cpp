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

    permission("boreal.command")
        .description("Allow users to use all commands provided by this example plugin")
        .children("boreal.command.flyspeed", true);

    permission("boreal.command.flyspeed")
        .description("Allow users to use the flyspeed command")
        .default_(endstone::PermissionDefault::Operator);
}

ssize_t tick_hook(long current_tick)
{
    return 1;
}

int install_hooks(ssize_t startingAddress)
{

    long current_tick = 0;
    funchook_set_debug_file("funchook-debug");
    funchook_t *funchook = funchook_create();
    printf("funchook");
    int rv;
    ssize_t tick = startingAddress + 130259312; // address of "_ZN5Level4tickEv"

    /* Prepare hooking.
     * The return value is used to call the original tick function
     * in tick_hook.
     */
    tick_func = (ssize_t(*)(long))tick;
    printf("funchook prepare");
    rv = funchook_prepare(funchook, (void **)tick_func(current_tick), (void*)tick_hook(current_tick));
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
