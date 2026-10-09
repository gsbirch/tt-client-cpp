#pragma once

#include <string>
#include <nlohmann/json.hpp>

#ifndef ENTITY_H
#define ENTITY_H

namespace tt {
    /**
     * An entity is an asset that represents a character, object,
     * place, or idea in a {@link tt::World story world}.
     * 
     * @author Gage Birchmeier
     */
    class Entity {
        public:
            /* Type used for JSON deserialization */
            static const std::string type;

            /* This asset's ID number, which is unique among other assets of the same type */
            int id;
            /* This asset's name, which is unique among other assets of the same type */
            std::string name;

            // default constructor for json deserialization
            Entity():id(0) {};

            /**
             * Returns true if this entity represents the player character in its story
             * world.
             * <p>
             * By default, the entity with ID number 0 is assumed to be the player
             * character.
             * 
             * @return true if this entity represents the player character, false
             * otherwise
             */
            bool isPlayer() const;

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
             * Returns a string representation of this action.
             * 
             * @returns a string describing this action
             */
            std::string toString() const;

            bool operator==(const Entity& rhs) const;

            friend void from_json(const nlohmann::json& j, Entity& msg);
            friend void to_json(nlohmann::json& j, const Entity& msg);
            friend std::ostream& operator<<(std::ostream& os, const Entity& a);

        private:
            /** A natural language description of this assignments */
            std::string description;
            /** The assignment's encoding */
            std::string code;
    };
    void from_json(const nlohmann::json& j, Entity& msg);
    void to_json(nlohmann::json& j, const Entity& msg);
    std::ostream& operator<<(std::ostream& os, const Entity& a);
}

#endif
