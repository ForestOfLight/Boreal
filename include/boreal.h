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
#include "TickCommandExecutor.h"
#include "FlyspeedCommandExecutor.h"

#include "PlayerQuitListener.h"

class Boreal : public endstone::Plugin {
public:
    std::unique_ptr<CanopyExtension> canopyExtension;

    void onEnable() override
    {
        void * baseAddress = getBaseAddress();
        int rv = install_hooks(baseAddress);
        if (rv != 0){
            getLogger().error("Failed to install hooks.");
        }

        this->canopyExtension = std::make_unique<CanopyExtension>(*this);
      
        if (auto *command = getCommand("flyspeed")) {
            command->setExecutor(std::make_unique<FlyspeedCommandExecutor>());
        }

        TickSpeed::logger = &getLogger();
        TickSpeed::server = &getServer();
        if (auto *command = getCommand("tick")){
            command->setExecutor(std::make_unique<TickCommandExecutor>(*this));
        }

        playerQuitListener = std::make_unique<PlayerQuitListener>(*this);
        registerEvent(&PlayerQuitListener::onPlayerQuit, *playerQuitListener, endstone::EventPriority::High);
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
