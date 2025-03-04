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

#include "PlayerQuitListener.h"

class Boreal : public endstone::Plugin {
public:
    std::unique_ptr<CanopyExtension> canopyExtension;

    void onLoad() override
    {
        getLogger().info("Boreal loaded successfully");
    }

    void onEnable() override
    {
        void * baseAddress = getBaseAddress();
        getLogger().info("Boreal enabled!");
        int rv = install_hooks(baseAddress);
        if (rv != 0){
            getLogger().error("Failed to install hooks.");
        }
        getLogger().info("Hooks installed!");

        this->canopyExtension = std::make_unique<CanopyExtension>(*this);

        TickSpeed::logger = &getLogger();
        TickSpeed::server = &getServer();
        if (auto *command = getCommand("tick")){
            command->setExecutor(std::make_unique<TickCommandExecutor>());
        }

        playerQuitListener = std::make_unique<PlayerQuitListener>(*this);
        registerEvent(&PlayerQuitListener::onPlayerQuit, *playerQuitListener, endstone::EventPriority::High);
    }

    void onDisable() override
    {
        getLogger().info("onDisable is called");
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

    void onServerLoad(endstone::ServerLoadEvent &event)
    {
        getLogger().info("{} is passed to ExamplePlugin::onServerLoad", event.getEventName());
    }

private:
    std::unique_ptr<PlayerQuitListener> playerQuitListener;
};
