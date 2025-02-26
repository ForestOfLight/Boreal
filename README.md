# Boreal
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
