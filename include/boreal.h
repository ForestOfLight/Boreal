#include "endstone/server.h"
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <endstone/color_format.h>
#include <endstone/command/plugin_command.h>
#include <endstone/event/server/server_command_event.h>
#include <endstone/event/server/server_load_event.h>
#include <endstone/player.h>
#include <endstone/plugin/plugin.h>
#include <endstone/level/level.h>
/* #include <bedrock/world/level/level.h> */
#include <memory>
#include <string>
#include <variant>
#include <vector>
#include "hook.h"

/* class TickHandler : public Level { */

/*     public: */ 
/*         void tick() { */
/*             std::printf("a"); */
/*         } */

/* }; */


class Boreal : public endstone::Plugin {
public:
    void onLoad() override
    {
        getLogger().info("Boreal loaded succesfully");
    }

    void onEnable() override
    {
        void * startAddr = getAddr();

        getLogger().info("Boreal enabled!");
        getLogger().info("Installing hooks");
        int rv = install_hooks(startAddr);
        if (rv != 0){
            getLogger().error("Couldn't install hooks");
        }
        getLogger().info("Hooks Installed!");
    }

    void onDisable() override
    {
        getLogger().info("onDisable is called");
    }

    bool onCommand(endstone::CommandSender &sender, const endstone::Command &command,
                   const std::vector<std::string> &args) override
    {
        if (command.getName() == "flyspeed") {
            if (args.size() == 0) {
                sender.sendMessage("Flyspeed set to default");
                sender.asPlayer()->setFlySpeed(0.05f);
                return true;
            }
            sender.sendMessage("Flyspeed set to: " + args[0] + "x");
           /* sender.asPlayer()->setFlying(false); */
            sender.asPlayer()->setFlySpeed(std::stof(args[0]) * 0.05);
            return true;
        }


        if (command.getName() == "tick"){

            if (args[0] == "freeze") {
                Tick::tickFreeze = true;
                return true;
            }

            if (args[0] == "unfreeze") {
                Tick::tickFreeze = false;
                return true;
            }

            if (args[0] == "step") {
                Tick::stepCounter = std::stoi(args[1]);
                if (Tick::stepCounter == 0) {
                    Tick::stepCounter = 1;
                }
                return true;
            }
            

            if (args[0] == "slow") {
                Tick::tickSlowdown = std::stoi(args[1]);
                return true;
            }

            if (args[0] == "sprint") {
                /* Tick::tickAccel = std::stoi(args[1]); */
                sender.sendMessage("TODO: Implement tick warp");
                return true;
            }
        }

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
};
