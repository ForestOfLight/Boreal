#pragma once

#include <endstone/command/command_executor.h>
#include <string>
#include "Tick.h"
#include <endstone/server.h>

class TickCommandExecutor : public endstone::CommandExecutor {
public:
    bool onCommand(endstone::CommandSender &sender, const endstone::Command &command,
             const std::vector<std::string> &args) override {
        std::string feedback = "Usage: /tick <query|rate|freeze|unfreeze|step|sprint> [<rate>|<time>]";
        if (args[0] == "query") {
            feedback = query();
        }
        if (args[0] == "rate") {
            feedback = rate(std::stof(args[1]));
        }
        if (args[0] == "freeze") {
            feedback = freeze();
        }
        if (args[0] == "unfreeze") {
            feedback = unfreeze();
        }
        if (args[0] == "step") {
            if (args[1] == "stop") {
                feedback = step(0);
            } else {
                feedback = step(std::stoi(args[1]));
            }
        }
        if (args[0] == "sprint") {
            if (args[1] == "stop") {
                feedback = sprint(0);
            } else {
                feedback = sprint(std::stoi(args[1]));
            }
        }
        sender.sendMessage(feedback);
        return true;
    }

private:
    std::string query() {
        return "Current target tick rate: " + std::to_string(Tick::targetTickRate);
    }

    std::string freeze() {
        Tick::storedTickRate = Tick::targetTickRate;
        Tick::targetTickRate = 0.0;
        return "Tick rate frozen.";
    }

    std::string unfreeze() {
        Tick::targetTickRate = Tick::storedTickRate;
        return "Tick rate unfrozen.";
    }

    std::string step(int ticks) {
        if (ticks == 0) {
            ticks = 1;
        }
        Tick::stepTicks = ticks;
        return "Stepping " + std::to_string(ticks) + " ticks";
    }

    std::string rate(float tps) {
        Tick::storedTickRate = 20.0;
        Tick::targetTickRate = tps;
        return "Tick rate set to " + std::to_string(tps);
    }

    std::string sprint(int ticks) {
        if (ticks == 0) {
            ticks = 1;
        }
        Tick::sprintTicks = ticks;
        /* Tick::tickAccel = std::stoi(args[1]); */
        return "TODO: Implement tick warp";
    }
};
