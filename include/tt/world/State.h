#include <string>
#include <vector>
#include <tt/world/Signature.h>
#include <tt/world/Assignment.h>

#pragma once

#ifndef STATE_H
#define STATE_H

namespace tt {
    /**
     * A state is an {@link tt::Assignment assignment} of {@link tt::Value values} to each
     * {@link tt::Variable variable} in a {@link tt::World story world}.
     * 
     * @author Gage Birchmeier
     */
    class State {
        public:
            // default constructor for json deserialization
            State() {};

            /**
             * Returns a string representation of this action.
             * 
             * @returns a string describing this action
             */
            std::string toString() const;

            /**
             * Returns a human-readable natural language description of the object.
             * 
             * @return a human-readable natural language description of the object
             */
            const std::string& getDescription() const;

            /**
             * Returns a string of {@code 1}'s and {@code 0}'s that uniquely represents
             * this object among other objects of the same type in the same story world.
             * Every encoded object of the same type should return a string of the same
             * length, even if it must be padded with {@code 0}'s.
             * 
             * @return a string of 1's and 0's
             */
            const std::string& getCode() const;

            /**
             * Returns an unmodifiable list of assignments of a value to each variable
             * in the state.
             * 
             * @return an unmodifiable list of variable assignments
             */
            const std::vector<const Assignment*> getAssignments() const;

            /**
             * Returns the value assigned to the given variable, or nullptr if the variable
             * does not exist in this state or is assigned a null value.
             * 
             * @param variable a variable whose assigned value is desired
             * @return the value assigned to the given variable in this state
             */
            const Value* get(const Variable* variable) const;

            bool operator==(const State& rhs) const;

            friend void from_json(const nlohmann::json& j, State& obj);
            friend void to_json(nlohmann::json& j, const State& obj);
            friend std::ostream& operator<<(std::ostream& os, const State& a);
        private:
            /** A natural language description of the state */
            std::string description;   
            /** The {@link Encoded encoding} of the state */ 
            std::string code;
            /** An vector of assignments for each variable in the story world */
            std::vector<std::unique_ptr<Assignment>> assignments;
            /** A cache of the usable vector */
            mutable std::vector<const Assignment *> assignmentsPtrs;
    };

    void from_json(const nlohmann::json& j, State& obj);
    void to_json(nlohmann::json& j, const State& obj);
    std::ostream& operator<<(std::ostream& os, const State& a);
}

#endif