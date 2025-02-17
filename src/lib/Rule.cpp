#include "lib/Rule.h"

Rule::Rule(std::string name, std::string description, std::vector<Rule> contingentRules,
           std::vector<Rule> independentRules) {
    this->identifier = name;
    this->description = description;
    this->contingentRules = contingentRules;
    this->independentRules = independentRules;
}

Rule::~Rule() {
    this->contingentRules.clear();
    this->independentRules.clear();
}

std::string Rule::getID() {
    return this->identifier;
}

std::string Rule::getDescription() {
    return this->description;
}

std::vector<Rule> Rule::getContingentRules() {
    return this->contingentRules;
}

std::vector<Rule> Rule::getIndependentRules() {
    return this->independentRules;
}

bool Rule::getValue() {
    // get stored value
}

void Rule::setValue(bool value) {
    // set stored value
}
