#include <string>
#include <nlohmann/json.hpp>

#pragma once

#ifndef ENTITY_H
#define ENTITY_H

namespace tt {
    class Entity {
        public:
            // used for json
            static const std::string type;

            int id;
            std::string name;
            std::string description;
            std::string code;

            /**
             * Constructs a new entity.
             * 
             * @param id the entity's unique ID number
             * @param name the entity's unique name
             * @param description the entity's description
             * @param code the entity's code
             */
            Entity(int id, std::string name, std::string description, std::string code);

            // default constructor for json deserialization
            Entity() {};
            
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

            std::string toString() const;
            friend std::ostream& operator<<(std::ostream& os, const Entity& a);

        private:
            
    };
    void from_json(const nlohmann::json& j, Entity& msg);
    void to_json(nlohmann::json& j, const Entity& msg);
    std::ostream& operator<<(std::ostream& os, const Entity& a);
}

#endif
