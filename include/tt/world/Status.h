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
     * A status contains the full history of a session
     * in a {@link tt::World story world}, a description of its current 
     * {@link tt::State state}, and a list of {@link tt::Turn turns} that 
     * can be taken next (if any).
     * 
     * @author Gage Birchmeier
     */
    class Status {
        public:
            /** The role of the participant to whom this status is being described */
            Role role;

            // default constructor for json deserialization
            Status(): state(nullptr), ending(nullptr), role(Role::NONE) {}

            /**
             * Returns a string representation of this action.
             * 
             * @returns a string describing this action
             */
            std::string toString() const;

            /**
             * Returns an unmodifiable list of all turns that have happened so far in
             * the session.
             * 
             * @return a vector of turns
             */
            const std::vector<const Turn *>& getHistory() const;
            /**
             * Returns an unmodifiable list of all the {@link tt::Entity entities} that
             * {@link #role this role} can current see. Each one will have a 
             * {@link Entity#getDescription() natural language description} that gives some
             * helpful context about it. Note that not all entities in the story world
             * will be in this list; if an entity cannot be seen it is excluded.
             * 
             * @return a vector of all visible entities and their descriptions
             */
            const std::vector<const Entity *> getDescriptions() const;
            /**
             * Returns an unmodifiable list of next {@link tt::Turn turns} that
             * {@link #role this role} can take next in the session. This list 
             * will be empty if the story had ended or if it is not this role's turn to act.
             * 
             * @return a vector of available next turns
             */
            const std::vector<const Turn *> getChoices() const;
            /**
             * Returns a state that gives the current value of all the story world's
             * {@link tt::Variable variables}.
             * 
             * @return the current state of the story world
             */
            const State* getState() const;
            /**
             * Returns the ending the session's story has reached, or null if the story
             * has not yet ended.
             * 
             * @return the story's ending, or null if it has not ended
             */
            const Ending* getEnding() const;

            friend void from_json(const nlohmann::json& j, Status& obj);
            friend void to_json(nlohmann::json& j, const Status& obj);
            friend std::ostream& operator<<(std::ostream& os, const Status& a);

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