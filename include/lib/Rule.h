#pragma once

#include <string>
#include <vector>

class Rule {
    public:
        Rule(std::string name, std::string description, std::vector<Rule> contingentRules, std::vector<Rule> independentRules);
        Rule(std::string name, std::string description, std::vector<Rule> contingentRules);
        Rule(std::string name, std::string description);
        ~Rule();

        std::string getID();
        std::string getDescription();
        std::vector<Rule> getContingentRules();
        std::vector<Rule> getIndependentRules();
        bool getValue();
        void setValue(bool value);

    private:
        std::string identifier;
        std::string description;
        std::vector<Rule> contingentRules;
        std::vector<Rule> independentRules;
        bool value; // Needs to be saved across sessions
};
