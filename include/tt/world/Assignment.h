#pragma once

#include <string>
#include <tt/world/Variable.h>
#include <tt/world/Signature.h>

#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

namespace tt {
    /**
     * An assignment is a logical formula that asserts that some
     * {@link tt::Variable variable} in a {@link tt::World story world} has a 
     * {@link tt::Value value}.
     * 
     * @author Gage Birchmeier
     */
    class Assignment {
        public:

            /** The value assigned to the variable */
            Value value;

            /**
             * Whether or not this assignment can currently be observed. When an
             * assignment can be observed, it is known to be the case. When it cannot
             * be observed, it represents an assumed or last known value of the
             * variable.
             */
            bool visible;

            Assignment(): variable(nullptr), value(std::monostate()) {};

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
             * Returns the variable being assigned a value.
             * 
             * @return a pointer to the variable being assigned a value
             */
            const Variable* getVariable() const;

            bool operator==(const Assignment& rhs) const;

            friend void from_json(const nlohmann::json& j, Assignment& obj);
            friend void to_json(nlohmann::json& j, const Assignment& obj);
            friend std::ostream& operator<<(std::ostream& os, const Assignment& a);

        private:
            /** A natural language description of this assignments */
            std::string description;
            /** The assignment's encoding */
            std::string code;
            /** The variable that is being assigned a value */
            std::unique_ptr<Variable> variable;
    };

    void from_json(const nlohmann::json& j, Assignment& obj);
    void to_json(nlohmann::json& j, const Assignment& obj);
    std::ostream& operator<<(std::ostream& os, const Assignment& a);
}

#endif