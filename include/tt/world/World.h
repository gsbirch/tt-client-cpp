#include <string>
#include <vector>
#include <tt/world/Entity.h>
#include <tt/world/Variable.h>
#include <tt/world/Action.h>
#include <tt/world/Ending.h>

#pragma once

#ifndef WORLD_H
#define WORLD_H

namespace tt {
    /**
     * A story world is a collection of assets needed for two partners
     * in a session to tell a collaborative story.
     * 
     * @author Gage Birchmeier
     */
    class World {
        public:
            /** The unique name of this story world */
            std::string name;

            // default constructor for JSON deserialization
            World() {};

            /**
             * Returns a string representation of this action.
             * 
             * @returns a string describing this action
             */
            std::string toString() const;

            /**
             * Returns an unmodifiable list of all entities defined in this story world
             * ordered by the ID numbers.
             * 
             * @return a vector of entities defined in this story world
             */
            const std::vector<const Entity *>& getEntities() const;

            /**
             * Returns an unmodifiable list of all variables defined in this story
             * world ordered by the ID numbers.
             * 
             * @return a vector of variables defined in this story world
             */
            const std::vector<const Variable *>& getVariables() const;

            /**
             * Returns an unmodifiable list of all actions defined in this story world
             * ordered by the ID numbers.
             * 
             * @return a vector of actions defined in this story world
             */
            const std::vector<const Action *>& getActions() const;

            /**
             * Returns an unmodifiable list of all endings defined in this story world
             * ordered by the ID numbers.
             * 
             * @return a vector of endings defined in this story world
             */
            const std::vector<const Ending *>& getEndings() const;

            friend std::ostream& operator<<(std::ostream& os, const World& a);
            friend void from_json(const nlohmann::json& j, World& obj);
            friend void to_json(nlohmann::json& j, const World& obj);

            private:
                /** This world's entities, as a vector */
                std::vector<std::unique_ptr<Entity>> entities;
                /** This world's variables, as a vector */
                std::vector<std::unique_ptr<Variable>> variables;
                /** This world's actions, as a vector */
                std::vector<std::unique_ptr<Action>> actions;
                /** This world's endings, as an array */
                std::vector<std::unique_ptr<Ending>> endings;

                mutable std::vector<const Entity *> entitiesPtrs;
                mutable std::vector<const Variable *> variablesPtrs;
                mutable std::vector<const Action *> actionsPtrs;
                mutable std::vector<const Ending *> endingsPtrs;
    };

    void from_json(const nlohmann::json& j, World& obj);
    void to_json(nlohmann::json& j, const World& obj);

    /**
     * Override stream insertion operator to allow printing the World class
     * 
     * @param os an output stream to insert into
     * @param a a reference to an world object
     * @returns the output stream referenced
     */
    std::ostream& operator<<(std::ostream& os, const World& a);
}

#endif