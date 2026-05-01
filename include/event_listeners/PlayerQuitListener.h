#pragma once

#include <endstone/endstone.hpp>
#include <endstone/event/player/player_quit_event.h>
#include "../classes/PlayerNoClip.h"

class PlayerQuitListener {
public:
    explicit PlayerQuitListener(endstone::Plugin &plugin) : plugin_(plugin) {}

    void onPlayerQuit(endstone::PlayerQuitEvent &event) {
        TickSpeed::onPlayerQuit(event);
        NativePlayerCache::remove(event.getPlayer().getId());
        PlayerNoClip::clearState(event.getPlayer().getId());
    }

private:
    endstone::Plugin &plugin_;
};