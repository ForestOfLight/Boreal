#pragma once

#include <endstone/endstone.hpp>
#include <endstone/event/player/player_quit_event.h>

class PlayerQuitListener {
public:
    explicit PlayerQuitListener(endstone::Plugin &plugin) : plugin_(plugin) {}

    void onPlayerQuit(endstone::PlayerQuitEvent &event) {
        TickSpeed::onPlayerQuit(event);
    }

private:
    endstone::Plugin &plugin_;
};