#pragma once

#include <string>
#include <vector>

class Rule {
    public:
        Rule(std::string name, std::string description, std::vector<std::string> contingentRules,
           std::vector<std::string> independentRules) {
            this->identifier = name;
            this->description = description;
            this->contingentRules = contingentRules;
            this->independentRules = independentRules;
        }

        ~Rule() {
            this->contingentRules.clear();
            this->independentRules.clear();
        }

        std::string getID() {
            return this->identifier;
        }

        std::string getDescription() {
            return this->description;
        }

        std::vector<std::string> getContingentRules() {
            return this->contingentRules;
        }

        std::vector<std::string> getIndependentRules() {
            return this->independentRules;
        }

        bool getValue() {
            throw std::runtime_error("Not implemented");
            // get stored value
        }

        void setValue(bool value) {
            throw std::runtime_error("Not implemented");
            // set stored value
        }

    private:
        std::string identifier;
        std::string description;
        std::vector<std::string> contingentRules;
        std::vector<std::string> independentRules;
};
