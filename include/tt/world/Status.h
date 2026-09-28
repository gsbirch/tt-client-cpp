#include <vector>
#include <memory>
#include <tt/Role.h>
#include <tt/world/Turn.h>
#include <tt/world/State.h>
#include <tt/world/Ending.h>

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
            
            

            // default constructor for json deserialization
            Status(): state(nullptr), ending(nullptr), role(Role::NONE) {}

            std::string toString() const;

            const std::vector<const Turn *>& getHistory() const;
            const std::vector<const Entity *> getDescriptions() const;
            const std::vector<const Turn *> getChoices() const;
            const State* getState() const;
            const Ending* getEnding() const;

            friend std::ostream& operator<<(std::ostream& os, const Status& a);

            friend void from_json(const nlohmann::json& j, Status& obj);
            friend void to_json(nlohmann::json& j, const Status& obj);

        private:
            /** All turns that have happened so far in the session */
            std::vector<std::unique_ptr<Turn>> history;
            /** Descriptions of the entities that are currently visible to the role */
            std::vector<std::unique_ptr<Entity>> descriptions;
            /** A list of turns the role can take next (which may be none) */
            std::vector<std::unique_ptr<Turn>> choices;
            /** The current values of all variables the story world */
            std::unique_ptr<State> state;
            /**
             * The ending the session's story has reached, or null if the story has not
             * yet ended
             */
            std::unique_ptr<Ending> ending;

            mutable std::vector<const Turn *> historyPtrs;
            mutable std::vector<const Entity *> descriptionsPtrs;
            mutable std::vector<const Turn *> choicesPtrs;
    };

    void from_json(const nlohmann::json& j, Status& obj);
    void to_json(nlohmann::json& j, const Status& obj);
    std::ostream& operator<<(std::ostream& os, const Status& a);
}

#endif