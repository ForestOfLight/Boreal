//#include "lib/canopyExtension.h"
//
//#include <string>
//#include <algorithm>
//#include <stdexcept>
//#include <fmt/format.h>
//#include <unordered_map>
//
//
//
//CanopyExtension::CanopyExtension(std::string name, std::string version, std::string author, std::string description, endstone::Plugin &plugin)
//    : plugin(plugin) {
//    this->id = this->makeID(name);
//    this->name = name;
//    this->version = version;
//    this->author = author;
//    this->description = description;
//    this->commands = std::unordered_map<std::string, Command>();
//    this->rules = std::unordered_map<std::string, Rule>();
//    this->isRegistrationReady = false;
//
//    this->registerExtension();
//    this->handleRuleValueRequests();
//    this->handleRuleValueSetters();
//}
//
//CanopyExtension::~CanopyExtension() {
//    this->commands.clear();
//    this->rules.clear();
//}
//
//void CanopyExtension::addCommand(Command command) {
//    this->commands.insert({command.getName(), command});
//    if (this->isRegistrationReady) {
//        this->registerCommand(command);
//    }
//}
//
//void CanopyExtension::addRule(Rule rule) {
//    this->rules.insert({rule.getID(), rule});
//    if (this->isRegistrationReady) {
//        this->registerRule(rule);
//    }
//}
//
//bool CanopyExtension::getRuleValue(std::string ruleID) {
//    return this->rules.at(ruleID).getValue();
//}
//
//void CanopyExtension::setRuleValue(std::string ruleID, bool value) {
//    this->rules.at(ruleID).setValue(value);
//}

//std::string CanopyExtension::makeID(std::string name) {
//    std::string id = name;
//    std::transform(id.begin(), id.end(), id.begin(), ::tolower);
//    id.erase(std::remove_if(id.begin(), id.end(), [](char c) {
//        return !std::isalnum(c) && c != ' ';
//    }), id.end());
//    std::replace(id.begin(), id.end(), ' ', '_');
//    if (id.empty() || id.length() > 32) {
//        throw std::invalid_argument("Invalid Name: Name must be non-empty and up to 32 characters long");
//    }
//    return id;
//}
//
//void CanopyExtension::registerExtension() {
//    ipc::once<Ready>("canopyExtension:ready", [this](const Ready &ready) {
//        RegisterExtension registerExtension;
//        registerExtension.name = this->name;
//        registerExtension.version = this->version;
//        registerExtension.author = this->author;
//        registerExtension.description = this->description;
//        registerExtension.isEndstone = true;
//        ipc::send<RegisterExtension>("canopyExtension:registerExtension", registerExtension, this->plugin);
//    });
//    ipc::once<Ready>(fmt::format("canopyExtension:{}:ready", this->id), [this](const Ready &ready) {
//        this->isRegistrationReady = true;
//        for (auto &command : this->commands) {
//            this->registerCommand(command.second);
//        }
//        for (auto &rule : this->rules) {
//            this->registerRule(rule.second);
//        }
//    });
//}
//
//void CanopyExtension::registerCommand(Command command) {
//    RegisterCommand registerCommand;
//    registerCommand.name = command.getName();
//    registerCommand.description = command.getDescription();
//    registerCommand.usage = command.getUsage();
//    registerCommand.args = command.getArgs();
//    registerCommand.contingentRules = command.getContingentRules();
//    registerCommand.adminOnly = command.isAdminOnly();
//    registerCommand.helpEntries = command.getHelpEntries();
//    registerCommand.helpHidden = command.isHelpHidden();
//    registerCommand.extensionName = this->name;
//    ipc::send<RegisterCommand>(fmt::format("canopyExtension:{}:registerCommand", this->id), registerCommand, this->plugin);
//}
//
//void CanopyExtension::registerRule(Rule rule) {
//    RegisterRule registerRule;
//    registerRule.identifier = rule.getID();
//    registerRule.description = rule.getDescription();
//    registerRule.contingentRules = rule.getContingentRules();
//    registerRule.independentRules = rule.getIndependentRules();
//    registerRule.extensionName = this->name;
//    ipc::send<RegisterRule>(fmt::format("canopyExtension:{}:registerRule", this->id), registerRule, this->plugin);
//}
//
//void CanopyExtension::handleRuleValueRequests() {
//    ipc::handle<RuleValueRequest, RuleValueResponse>(fmt::format("canopyExtension:{}:ruleValueRequest", this->id), [this](const RuleValueRequest &request) {
//        RuleValueResponse response;
//        response.value = this->getRuleValue(request.ruleID);
//        return response;
//    }, this->plugin);
//}
//
//void CanopyExtension::handleRuleValueSetters() {
//    ipc::on<RuleValueSet>(fmt::format("canopyExtension:{}:ruleValueSet", this->id), [this](const RuleValueSet &ruleValueSet) {
//        this->setRuleValue(ruleValueSet.ruleID, ruleValueSet.value);
//    });
//}
