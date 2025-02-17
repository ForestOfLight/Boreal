#include "lib/canopyIntegration.h"

#include <string>
#include <algorithm>
#include <stdexcept>

CanopyIntegration::CanopyIntegration(std::string name, std::string version, std::string author, std::string description) {
    this->id = this->makeID(name);
    this->name = name;
    this->version = version;
    this->author = author;
    this->description = description;
    this->isRegistrationReady = false;
}

CanopyIntegration::~CanopyIntegration() {
    this->commands.clear();
    this->rules.clear();
}

void CanopyIntegration::addCommand(Command command) {
    this->commands[command.getName()] = command;
    if (this->isRegistrationReady) {
        this->registerCommand(command);
    }
}

void CanopyIntegration::addRule(Rule rule) {
    this->rules[rule.getID()] = rule;
    if (this->isRegistrationReady) {
        this->registerRule(rule);
    }
}

bool CanopyIntegration::getRuleValue(std::string ruleID) {
    return this->rules[ruleID].getValue();
}

void CanopyIntegration::setRuleValue(std::string ruleID, bool value) {
    this->rules[ruleID].setValue(value);
}

std::string CanopyIntegration::makeID(std::string name) {
    std::string id = name;
    std::transform(id.begin(), id.end(), id.begin(), ::tolower);
    id.erase(std::remove_if(id.begin(), id.end(), [](char c) {
        return !std::isalnum(c) && c != ' ';
    }), id.end());
    std::replace(id.begin(), id.end(), ' ', '_');
    if (id.empty() || id.length() > 32) {
        throw std::invalid_argument("Invalid Name: Name must be non-empty and up to 32 characters long");
    }
    return id;
}

void CanopyIntegration::registerIntegration() {
//    ipc::once("canopyIntegration:ready", ...) {
//        this->isRegistrationReady = true;
//        for (const auto &command : this->commands) {
//           this->registerCommand(command.second);
//        }
//        for (const auto &rule : this->rules) {
//            this->registerRule(rule.second);
//        }
//    });
}

void CanopyIntegration::registerCommand(Command command) {
//    ipc::send<CanopyCommandPacket>(fmt.format("canopyIntegration:", this->id, ":registerCommand") ...);
}

void CanopyIntegration::registerRule(Rule rule) {
//    ipc::send<CanopyRulePacket>(fmt.format("canopyIntegration:", this->id, ":registerRule") ...);
}

void CanopyIntegration::handleRuleValueRequests() {
//    ipc::on<CanopyRulePacket>(fmt.format("canopyIntegration:", this->id, ":ruleValueRequest") ...);
}

void CanopyIntegration::handleRuleValueSetters() {
//    ipc::on<CanopyRulePacket>(fmt.format("canopyIntegration:", this->id, ":ruleValueSet") ...);
}
