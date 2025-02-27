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
        std::string message;
        if (TickSpeed::isSprinting())
            message += "§7The game is sprinting.\n";
        else if (TickSpeed::isFrozen)
            message += "§7The game is frozen.\n";
        else
            message += "§7The game is running normally.\n";

        message += fmt::format("§7Target tick rate: {:.1f} per second.", TickSpeed::targetTickRate);
        if (TickSpeed::isSprinting() || TickSpeed::isFrozen)
            message += " (Ignored, reference only)\n";
        else
            message += "\n";

        message += fmt::format("§7Average time per tick: {:.1f}ms", TickSpeed::server->getAverageMillisecondsPerTick());
        if (!TickSpeed::isSprinting())
            message += fmt::format(" (Target: {:.1f}ms)\n", 1000.0 / TickSpeed::targetTickRate);
        else
            message += "\n";
        sender.sendMessage(message);
    }

    void setRate(endstone::CommandSender &sender, const std::vector<std::string> &args) {
        double minRate = 0;
        double maxRate = 1000;
        double rate = std::stod(args[1]);
        if (rate <= minRate) {
            sender.sendMessage("§cThe tick rate must not be less than or equal to {:.1f}, found {}.", minRate, rate);
            return;
        }
        if (rate > maxRate) {
            sender.sendMessage("§cThe tick rate must not be greater than {:.1f}, found {}.", maxRate, rate);
            return;
        }
        TickSpeed::setRate(rate);
        sender.sendMessage("§7Set the target tick rate to {}.", args[1]);
    }

    void freeze(endstone::CommandSender &sender) {
        TickSpeed::freeze();
        sender.sendMessage("§7The game is frozen.");
    }

    void unfreeze(endstone::CommandSender &sender) {
        TickSpeed::unfreeze();
        sender.sendMessage("§7The game is running normally.");
    }

    void step(endstone::CommandSender &sender, const std::vector<std::string> &args) {
        int minAllowed = 1;
        int maxAllowed = 10000;
        int ticks = 1;
        if (args.size() > 1) {
            ticks = std::stoi(args[1]);
        }
        if (ticks < minAllowed) {
            sender.sendMessage("§cThe tick count must not be less than {}, found {}.", minAllowed, ticks);
            return;
        }
        if (ticks > maxAllowed) {
            sender.sendMessage("§cThe tick count must not be greater than {}, found {}.", maxAllowed, ticks);
            return;
        }
        TickSpeed::step(ticks);
        sender.sendMessage("§7Stepping {} tick(s).", std::to_string(ticks));
    }

    void sprint(endstone::CommandSender &sender, const std::vector<std::string> &args) {
        int ticks = 1;
        int minAllowed = 0;
        if (args.size() > 1) {
            ticks = std::stoi(args[1]);
        }
        if (ticks < minAllowed) {
            sender.sendMessage("§cThe tick count must not be less than {}, found {}.", minAllowed, ticks);
            return;
        }
        TickSpeed::sprint(sender, ticks);
    }
};
