#pragma once

#include <string>
#include <vector>

class Rule {
    public:
        Rule(std::string name, std::string description, std::vector<std::string> contingentRules = {},
           std::vector<Rule> independentRules = {});
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
