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

    static size_t startAdd;

    void onLoad() override
    {
        getLogger().info("Boreal loaded succesfully");
    }

    void onEnable() override
    {
        std::string addressRange;
        std::ifstream mapsFile("/proc/self/maps");
        if (mapsFile.is_open()) {
            std::string line;
            std::getline(mapsFile, line);
            size_t start = line.find("-");
            size_t end = line.find(" ");
            addressRange = line.substr(0, start);
            getLogger().info(addressRange);
            mapsFile.close();
        } else {
            getLogger().info("unable to open /proc/self/maps");
        }


        getLogger().info("Boreal enabled!");
        install_hooks((ssize_t)std::stol(addressRange));
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

        sender.sendErrorMessage("Unknown command: /{}", command.getName());
        return false;
    }

    void onServerLoad(endstone::ServerLoadEvent &event)
    {
        getLogger().info("{} is passed to ExamplePlugin::onServerLoad", event.getEventName());
    }
};
