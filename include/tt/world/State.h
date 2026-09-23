#include <string>
#include <vector>
#include <tt/world/Signature.h>

#pragma once

#ifndef STATE_H
#define STATE_H

namespace tt {
    class State {
        public:
            /** An vector of assignments for each variable in the story world */
            std::vector<const Assignment *> assignments;
            /** A natural language description of the state */
            std::string description;   
            /** The {@link Encoded encoding} of the state */ 
            std::string code;

            // default constructor for json deserialization
            State() {};

            std::string toString() const;

            friend std::ostream& operator<<(std::ostream& os, const State& a);
    };

    void from_json(const nlohmann::json& j, State& obj);
    void to_json(nlohmann::json& j, const State& obj);
}

#endif