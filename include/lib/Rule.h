#pragma once

#include <string>
#include <vector>

class Rule {
    public:
        Rule(std::string name, std::string description, std::vector<std::string> contingentRules,
           std::vector<std::string> independentRules);
        ~Rule();

        std::string getID();
        std::string getDescription();
        std::vector<std::string> getContingentRules();
        std::vector<std::string> getIndependentRules();
        bool getValue();
        void setValue(bool value);

    private:
        std::string identifier;
        std::string description;
        std::vector<std::string> contingentRules;
        std::vector<std::string> independentRules;
};

inline Rule::Rule(std::string name, std::string description, std::vector<std::string> contingentRules,
    std::vector<std::string> independentRules) {
    this->identifier = name;
    this->description = description;
    this->contingentRules = contingentRules;
    this->independentRules = independentRules;
}

inline Rule::~Rule() {
    this->contingentRules.clear();
    this->independentRules.clear();
}

inline std::string Rule::getID() {
    return this->identifier;
}

inline std::string Rule::getDescription() {
    return this->description;
}

inline std::vector<std::string> Rule::getContingentRules() {
    return this->contingentRules;
}

inline std::vector<std::string> Rule::getIndependentRules() {
    return this->independentRules;
}

inline bool Rule::getValue() {
    throw std::runtime_error("Not implemented");
    // get stored value
}

inline void Rule::setValue(bool value) {
    throw std::runtime_error("Not implemented");
    // set stored value
}
