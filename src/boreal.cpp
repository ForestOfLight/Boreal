// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.

#include "boreal.h"

#include <cstdio>
#include <hook.h>
#include <string>
#include <system_error>
#include <unordered_map>

// The ENDSTONE_PLUGIN macro defines the metadata for the plugin.
ENDSTONE_PLUGIN(/*name=*/"boreal", /*version=*/"0.1.1", /*main_class=*/Boreal)
{
    prefix = "Boreal";
    description = "Canopy Extension for Endstone";
    website = "https://github.com/ForestOfLight/Boreal";
    authors = {"R2leyser", "ForestOfLight"};

    command("tick")
        .description("Controls the tick rate of the game")
        .usages("/tick (query)<a: bool>")
        .usages("/tick (rate)<a: bool> <n: float>")
        .usages("/tick (freeze)<b: bool>")
        .usages("/tick (unfreeze)<b: bool>")
        .usages("/tick (step)<b: bool> [n: int]")
        .usages("/tick (sprint)<a: bool> <n: int>")
        .permissions("boreal.command.op");

    command("flyspeed")
        .description("A command to change your fly speed")
        .usages("/flyspeed [speed: float]")
        .aliases("fs")
        .permissions("boreal.command.op");

    command("loadchunks")
        .description("Enables/disables player chunk loading")
        .usages("/loadchunks <a: bool>")
        .permissions("boreal.command.op");

    permission("boreal.command")
        .description("Allow users to use all commands provided by this plugin")
        .children("boreal.command.op", true);

    permission("boreal.command.op")
        .description("Set the command permission to op only")
        .default_(endstone::PermissionDefault::Operator);
}