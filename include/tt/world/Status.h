#include <vector>
#include <memory>
#include <tt/Role.h>
#include <tt/world/Turn.h>
#include <tt/world/State.h>

#pragma once

#ifndef STATUS_H
#define STATUS_H

namespace tt {
    /**
     * A status contains the full history of a {@link edu.uky.cs.nil.tt.Session
     * session} in a {@link World story world}, a description of its current {@link
     * State state}, and a list of {@link Turn turns} that can be taken next (if
     * any).
     * 
     * @author Stephen G. Ware
     */
    class Status {
        public:
            /** The role of the participant to whom this status is being described */
            Role role;
            /** All turns that have happened so far in the session */
            std::vector<const Turn *> history;
            /** Descriptions of the entities that are currently visible to the role */
            std::vector<const Entity *> descriptions;
            /** A list of turns the role can take next (which may be none) */
            std::vector<const Turn *> choices;
            /** The current values of all variables the story world */
            const State *state;
            /**
             * The ending the session's story has reached, or null if the story has not
             * yet ended
             */
            const Ending *ending;

            // default constructor for json deserialization
            Status(): state(nullptr), ending(nullptr), role(Role::NONE) {}

            std::string toString() const;

            friend std::ostream& operator<<(std::ostream& os, const Status& a);

            friend void from_json(const nlohmann::json& j, Status& obj);
            friend void to_json(nlohmann::json& j, const Status& obj);

        private:
            

            
    };

    void from_json(const nlohmann::json& j, Status& obj);
    void to_json(nlohmann::json& j, const Status& obj);
}

#endif