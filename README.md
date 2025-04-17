<div align="center">
    <a href="./Boreal-Logo.png">
        <img src="./Boreal-Logo.png" alt="Boreal Icon" width="100" height="100">
    </a>
    <p><b>Boreal</b></p>

[![GitHub Downloads](https://img.shields.io/github/downloads/ForestOfLight/Boreal/total?label=Github%20downloads&logo=github)](https://github.com/ForestOfLight/Boreal/releases/latest)
[![Curseforge Downloads](https://cf.way2muchnoise.eu/full_......._downloads.svg)](https://www.curseforge.com/minecraft-bedrock/addons/canopy)
[![Minecraft - Version](https://img.shields.io/badge/Minecraft-v1.21.70_(Bedrock)-brightgreen)](https://feedback.minecraft.net/hc/en-us/sections/360001186971-Release-Changelogs)
[![Discord](https://badgen.net/discord/members/9KGche8fxm?icon=discord&label=Discord&list=what)](https://discord.gg/9KGche8fxm)
</div>

An Endstone plugin for technical minecraft servers.

Can run standalone, but also integrates with [Canopy](https://github.com/ForestOfLight/Canopy).

---

## Usage

### Tick Command

**Usage: `/tick rate <tps: float>`**  
Changes the world's tick speed.

**Usage: `/tick <freeze/unfreeze>`**  
Starts or stops the tick speed.

**Usage: `/tick step [num: int]`**  
Makes the tick speed run for a few ticks while frozen. Defaults to 1 if the number of steps is unspecified.

**Usage: `/tick sprint <num: int>`**  
Makes the tick speed run as fast as possible for the desired number of ticks.

**Usage: `/tick query`**  
Shows the current tick speed.

### Flyspeed Command

**Usage: `/flyspeed [multiplier: float]`**  
Changes your fly speed. Defaults to 1 if the multiplier is unspecified.

---

## Deps:

https://github.com/kubo/funchook
https://github.com/gdabah/distorm/

## Roadmap

0. Integration with Canopy
0. No-clip
0. In-depth performance profiler
