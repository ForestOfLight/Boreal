#pragma once

#include "endstone/server.h"
#include <string>
#include <endstone/command/plugin_command.h>
#include <endstone/event/server/server_command_event.h>
#include <endstone/event/server/server_load_event.h>
#include <endstone/plugin/plugin.h>
#include <endstone/level/level.h>
#include <memory>
#include <variant>
#include <vector>

#include "lib/CanopyExtension.h"
#include "hook.h"
#include "classes/PlayerAbilities.h"
#include "commands/TickCommandExecutor.h"
#include "commands/FlyspeedCommandExecutor.h"
#include "commands/ForceOpenCommandExecutor.h"
#include "commands/LoadNearbyChunksCommandExecutor.h"
#include "commands/PistonPushLimitCommandExecutor.h"
#include "commands/NoClipCommandExecutor.h"

#include "events/PlayerQuitListener.h"

class Boreal : public endstone::Plugin {
public:
    std::unique_ptr<CanopyExtension> canopyExtension;

    void onEnable() override
    {
        this->canopyExtension = std::make_unique<CanopyExtension>(*this);

        TickSpeed::logger = &getLogger();
        TickSpeed::server = &getServer();
        if (auto *command = getCommand("tick")){
            command->setExecutor(std::make_unique<TickCommandExecutor>(*this));
        }

        if (auto *command = getCommand("flyspeed")) {
            command->setExecutor(std::make_unique<FlySpeedCommandExecutor>());
        }

        PlayersTickLevelChunks::logger = &getLogger();
        if (auto *command = getCommand("loadnearbychunks")) {
            command->setExecutor(std::make_unique<LoadNearbyChunksCommandExecutor>());
        }

        PistonPushLimit::logger = &getLogger();
        if (auto *command = getCommand("pistonpushlimit")) {
            command->setExecutor(std::make_unique<PistonPushLimitCommandExecutor>());
        }

        ForceOpenContainers::logger = &getLogger();
        if (auto *command = getCommand("forceopen")) {
            command->setExecutor(std::make_unique<ForceOpenCommandExecutor>());
        }

        PlayerAbilities::logger = &getLogger();

        PlayerNoClip::server = &getServer();
        if (auto *command = getCommand("noclip")) {
            command->setExecutor(std::make_unique<NoClipCommandExecutor>());
        }

        playerQuitListener = std::make_unique<PlayerQuitListener>(*this);
        registerEvent(&PlayerQuitListener::onPlayerQuit, *playerQuitListener, endstone::EventPriority::High);

        void * baseAddress = getBaseAddress();
        int rv = install_hooks(baseAddress);
        if (rv != 0){
            getLogger().error("Failed to install hooks.");
        }
    }

    bool onCommand(endstone::CommandSender &sender, const endstone::Command &command,
                   const std::vector<std::string> &args) override
    {
        if (command.getName() == "listactors"){
            auto actors = getServer().getLevel()->getActors();
            for (auto actor : actors){

                std::string actorName = actor->getName();

                if (actorName == "Villager"){
                    sender.sendMessage("Villager: " + std::to_string(actor->getId()));
                } else {
                    sender.sendMessage(actorName);
                }
            }
            return true;
        }

        sender.sendErrorMessage("Unknown command: /{}", command.getName());
        return false;
    }

private:
    std::unique_ptr<PlayerQuitListener> playerQuitListener;
};
