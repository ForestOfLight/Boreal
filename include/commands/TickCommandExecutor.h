#pragma once

#include <endstone/command/command_executor.h>
#include <endstone/plugin/plugin.h>
#include <string>
#include "../classes/Tick.h"

class TickCommandExecutor : public endstone::CommandExecutor {
public:
    explicit TickCommandExecutor(endstone::Plugin &plugin) : plugin(plugin) {}

    bool onCommand(endstone::CommandSender &sender, const endstone::Command &command,
             const std::vector<std::string> &args) override;

private:
    void query(endstone::CommandSender &sender);
    void setRate(endstone::CommandSender &sender, const std::vector<std::string> &args);
    void freeze(endstone::CommandSender &sender);
    void unfreeze(endstone::CommandSender &sender);
    void step(endstone::CommandSender &sender, const std::vector<std::string> &args);
    void sprint(endstone::CommandSender &sender, const std::vector<std::string> &args);

    endstone::Plugin &plugin;
};
