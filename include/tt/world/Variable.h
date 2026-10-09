#include <string>
#include <memory>
#include <tt/world/Signature.h>

#pragma once

#ifndef VARIABLE_H
#define VARIABLE_H

namespace tt {
    /**
     * A variable is an asset that represents a feature of
     * the {@link tt::World story world} {@link tt::State state} that can change. 
     * A {@link tt::State state} assign a {@link tt::Value value} to each of a 
     * story world's variables.
     * 
     * @author Gage Birchmeier
     */
    class Variable {
        public:
            /** This asset's ID number, which is unique among other assets of the same type */
            int id;
            /** This asset's name, which is unique among other assets of the same type */
            std::string name;

            /**
             * The name of the encoding this variable uses to encode its values
             */
            std::string encoding;

            // default constructor for JSON
            Variable(): id(0), signature(nullptr) {};

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
             * Returns the signature of this object.
             * 
             * @return a pointer to the signature of this Action.
             */
            const Signature* getSignature() const;

            bool operator==(const Variable& rhs) const;

            friend void from_json(const nlohmann::json& j, Variable& obj);
            friend void to_json(nlohmann::json& j, const Variable& obj);
            friend std::ostream& operator<<(std::ostream& os, const Variable& a);

        private:
            /** The asset's unique signature */
            std::unique_ptr<Signature> signature;
            /** A natural language description of this assignments */
            std::string description;
    };

    void from_json(const nlohmann::json& j, Variable& obj);
    void to_json(nlohmann::json& j, const Variable& obj);

    /**
     * Override stream insertion operator to allow printing the Variable class
     * 
     * @param os an output stream to insert into
     * @param a a reference to an variable object
     * @returns the output stream referenced
     */
    std::ostream& operator<<(std::ostream& os, const Variable& a);
}

#endif