<div align="center">
    <a href="./Boreal-Logo.png">
        <img src="./Boreal-Logo.png" alt="Boreal Icon" width="100" height="100">
    </a>
    <p><b>Boreal</b></p>

[![GitHub Downloads](https://img.shields.io/github/downloads/ForestOfLight/Boreal/total?label=Github%20downloads&logo=github)](https://github.com/ForestOfLight/Boreal/releases/latest)
[![Minecraft - Version](https://img.shields.io/badge/Minecraft-v1.21.130_(Bedrock)-brightgreen)](https://feedback.minecraft.net/hc/en-us/sections/360001186971-Release-Changelogs)
[![Discord](https://badgen.net/discord/members/9KGche8fxm?icon=discord&label=Discord&list=what)](https://discord.gg/9KGche8fxm)
</div>

An Endstone plugin for Technical Minecraft servers, specifically meant to fill in feature gaps in 
**[Canopy](https://github.com/ForestOfLight/Canopy)** created by a lack of access in the vanilla [Script API for Behavior Packs](https://learn.microsoft.com/en-us/minecraft/creator/scriptapi/?view=minecraft-bedrock-stable).

Can run standalone, but also integrates with **[Canopy](https://github.com/ForestOfLight/Canopy)**.

## Commands

### Tick

**Usage: `/tick rate <tps: float>`**  
Changes the world's tick speed.

**Usage: `/tick <freeze/unfreeze>`**  
Starts or stops the tick speed.

**Usage: `/tick step [num: int]`**  
Makes the tick speed run for a few ticks while frozen. Defaults to 1 if the number of steps is unspecified.

**Usage: `/tick sprint <num: int>`**  
Makes the tick speed run as fast as possible for the desired number of ticks. Use `/tick sprint 0` to interrupt the current sprint.

**Usage: `/tick query`**  
Shows the current tick speed.

### Flyspeed

**Usage: `/flyspeed [multiplier: float]`**  
Changes your creative mode flight speed. Defaults to 1 if the multiplier is unspecified.

### Load Nearby Chunks
**Usage: `/loadnearbychunks [shouldLoad: bool]`**
Toggles whether chunks nearby the player are ticked. Defaults to true if the parameter is unspecified.

## Piston Push Limit
**Usage: `/pistonpushlimit [n: int]`**
Changes the maximum number of blocks a piston can push. If the parameter is unspecified, it displays the current push limit.

## Supported Versions

| Minecraft Version | Boreal Version |
|-------------------|----------------|
| 1.21.130          | v0.1.1         |
| 1.21.80           | v0.1.0         |

## Roadmap

- [x] /tick Command
- [x] Horizontal Flight Speed Control
- [ ] Vertical Flight Speed Control
- [x] Disable Players Ticking Nearby Chunks
- [ ] No-clip
- [ ] In-depth performance profiler
- [x] Piston push limit
