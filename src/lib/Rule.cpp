#include "lib/Rule.h"

Rule::Rule(std::string name, std::string description, std::vector<std::string> contingentRules,
           std::vector<std::string> independentRules) {
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

std::vector<std::string> Rule::getContingentRules() {
    return this->contingentRules;
}

std::vector<std::string> Rule::getIndependentRules() {
    return this->independentRules;
}

bool Rule::getValue() {
    // get stored value
}

void Rule::setValue(bool value) {
    // set stored value
}
