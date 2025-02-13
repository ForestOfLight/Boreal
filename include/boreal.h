#include "endstone/server.h"
#include <cstdio>
#include <endstone/color_format.h>
#include <endstone/command/plugin_command.h>
#include <endstone/event/server/server_command_event.h>
#include <endstone/event/server/server_load_event.h>
#include <endstone/player.h>
#include <endstone/plugin/plugin.h>
#include <endstone/level/level.h>
#include <bedrock/world/level/level.h>
#include <memory>
#include <string>
#include <variant>
#include <vector>

class TickHandler : public Level {

    public: 
        void tick() {
            std::printf("a");
        }

};


class Boreal : public endstone::Plugin {
public:

    void onLoad() override
    {
        getLogger().info("Boreal loaded succesfully");
    }

    void onEnable() override
    {
        getLogger().info("Boreal enabled!");
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

        sender.sendErrorMessage("Unknown command: /{}", command.getName());
        return false;
    }

    void onServerLoad(endstone::ServerLoadEvent &event)
    {
        getLogger().info("{} is passed to ExamplePlugin::onServerLoad", event.getEventName());
    }
};
