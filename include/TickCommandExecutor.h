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
        sender.sendMessage("Query is not yet implemented.");
        // Should pull in endstone::Server for tick speed info because they already have it
    }

    void setRate(endstone::CommandSender &sender, const std::vector<std::string> &args) {
        double rate = 20.0;
        if (args.size() > 1)
            rate = std::stod(args[1]);
        Tick::setRate(rate);
        sender.sendMessage("Tick rate set to {}.", std::to_string(rate));
    }

    void freeze(endstone::CommandSender &sender) {
        Tick::freeze();
        sender.sendMessage("Tick rate now frozen.");
    }

    void unfreeze(endstone::CommandSender &sender) {
        Tick::unfreeze();
        sender.sendMessage("Tick rate runs normally.");
    }

    void step(endstone::CommandSender &sender, const std::vector<std::string> &args) {
        int ticks = 1;
        if (args.size() > 1 && args[1] == "0") {
            ticks = 0;
        } else if (args.size() > 1) {
            ticks = std::stoi(args[1]);
        }
        Tick::step(ticks);
        sender.sendMessage("Stepping {} ticks...", std::to_string(ticks));
    }

    void sprint(endstone::CommandSender &sender, const std::vector<std::string> &args) {
        int ticks = 1;
        if (args.size() > 1 && args[1] == "stop") {
            ticks = 0;
        } else if (args.size() > 1) {
            ticks = std::stoi(args[1]);
        }
        Tick::sprint(sender, ticks);
    }
};
