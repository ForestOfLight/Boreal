#pragma once

#include <endstone/command/command_executor.h>
#include <string>
#include "Tick.h"

class TickCommandExecutor : public endstone::CommandExecutor {
public:
    bool onCommand(endstone::CommandSender &sender, const endstone::Command &command,
             const std::vector<std::string> &args) override {
        if (args[0] == "query") {
            query(sender);
            return true;
        }
        if (args[0] == "rate") {
            setRate(sender, args);
            return true;
        }
        if (args[0] == "freeze") {
            freeze(sender);
            return true;
        }
        if (args[0] == "unfreeze") {
            unfreeze(sender);
            return true;
        }
        if (args[0] == "step") {
            step(sender, args);
            return true;
        }
        if (args[0] == "sprint") {
            sprint(sender, args);
            return true;
        }
        sender.sendMessage(command.getUsages().front());
        return true;
    }

private:
    void query(endstone::CommandSender &sender) {
        sender.sendMessage("§7Current tps is {:.1f}.", TickSpeed::server->getAverageTicksPerSecond());
    }

    void setRate(endstone::CommandSender &sender, const std::vector<std::string> &args) {
        TickSpeed::setRate(std::stod(args[1]));
        sender.sendMessage("§7Tick rate set to {}.", args[1]);
    }

    void freeze(endstone::CommandSender &sender) {
        TickSpeed::freeze();
        sender.sendMessage("§7Tick rate frozen.");
    }

    void unfreeze(endstone::CommandSender &sender) {
        TickSpeed::unfreeze();
        sender.sendMessage("§7Tick rate runs normally.");
    }

    void step(endstone::CommandSender &sender, const std::vector<std::string> &args) {
        int ticks = 1;
        if (args.size() > 1) {
            ticks = std::stoi(args[1]);
        }
        if (ticks <= 0) {
            ticks = 1;
        }
        TickSpeed::step(ticks);
        sender.sendMessage("§7Stepping {} ticks...", std::to_string(ticks));
    }

    void sprint(endstone::CommandSender &sender, const std::vector<std::string> &args) {
        int ticks = 1;
        if (args.size() > 1) {
            ticks = std::stoi(args[1]);
        }
        if (ticks <= 0) {
            ticks = 1;
        }
        TickSpeed::sprint(sender, ticks);
    }
};
