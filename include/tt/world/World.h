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
    class World {
        public:
            /** The unique name of this story world */
            std::string name;

            World() {};

            std::string toString() const;

            friend std::ostream& operator<<(std::ostream& os, const World& a);

            friend void from_json(const nlohmann::json& j, World& obj);
            friend void to_json(nlohmann::json& j, const World& obj);

            private:
                /** This world's entities, as a vector */
                std::vector<const Entity *> entities;

                /** This world's variables, as a vector */
                std::vector<const Variable *> variables;

                /** This world's actions, as a vector */
                std::vector<const Action *> actions;

                /** This world's endings, as an array */
                std::vector<const Ending *> endings;
    };

    void from_json(const nlohmann::json& j, World& obj);
    void to_json(nlohmann::json& j, const World& obj);
}

#endif