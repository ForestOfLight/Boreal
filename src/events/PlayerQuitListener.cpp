#include "events/PlayerQuitListener.h"

#include "classes/NativePlayerCache.h"
#include "classes/PlayerNoClip.h"
#include "classes/Tick.h"

void PlayerQuitListener::onPlayerQuit(endstone::PlayerQuitEvent &event) {
    TickSpeed::onPlayerQuit(event);
    NativePlayerCache::remove(event.getPlayer().getId());
    PlayerNoClip::clear(event.getPlayer().getId());
}
