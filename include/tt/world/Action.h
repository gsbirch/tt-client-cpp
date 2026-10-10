#pragma once

#include <string>
#include <tt/world/Entity.h>
#include <tt/world/Signature.h>
#include <unordered_set>
#include <tt/Role.h>

#ifndef ACTION_H
#define ACTION_H

namespace tt {
    /**
     * An action is a signed asset that causes a change in a
     * {@link tt::World story world's} {@link tt::State state}. The
     * agents in a session negotiate when actions happen
     * via {@link tt::Turn turns}.
     * 
     * @author Gage Birchmeier
     */
    class Action {
        public:
            /** This asset's ID number, which is unique among other assets of the same type */
            int id;
            /** This asset's name, which is unique among other assets of the same type */
            std::string name;
            

            /**
             * Constructs a new blank action.
             */
            Action():id(0), signature(nullptr) {};

            /**
             * Returns an unmodifiable set of entities in the story world representing
             * the characters who need to agree to take the action. An action with no
             * consenting characters represents an accident or happening that can occur
             * and time it is convenient for the story. An action with one consenting
             * character means it is taken by a single character. An action with two or
             * more consenting characters means it is a joint action taken by many
             * characters who all need to have a reason to do it. Note that a character
             * can be involved in an action without being a consenting character if the
             * action represents something that happens to the character against their
             * will.
             * 
             * @return the action's set of consenting characters
             */
            const std::unordered_set<const Entity*>& getConsenting();

            /**
             * Returns true if the given story role controls one or more of the 
             * {@link #getConsenting() consenting characters} for this action. An action with
             * no consenting character requires on the consent of the game master. An
             * action whose only consenting character is the player character requires
             * only the consent of the player. All other actions require the consent of
             * both roles.
             * 
             * @param role the role in question
             * @return true if the given role needs to consent to take this action
             */
            bool consents(Role role);

            /**
             * Returns a string representation of this object.
             * 
             * @returns a string describing this object
             */
            const std::string& toString() const;

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
             * Returns the signature of this object.
             * 
             * @return a pointer to the signature of this Action.
             */
            const Signature* getSignature() const;

            friend std::ostream& operator<<(std::ostream& os, const Action& a);

            friend void from_json(const nlohmann::json& j, Action& obj);
            friend void to_json(nlohmann::json& j, const Action& obj);

            bool operator==(const Action& rhs) const;
        private:
            /**
             * Entities representing characters in the story world who need to agree
             * to take the action
             */
            std::vector<std::unique_ptr<Entity>> consenting;
            /** The action's {@link #consenting consenting characters} as a set */
            std::unordered_set<const Entity*> consentingSet;

            /** A human-readable description of the asset */
            std::string description;
            /** The action's encoding */
            std::string code;
            /** The asset's unique signature */
            std::unique_ptr<Signature> signature;
    };

    void from_json(const nlohmann::json& j, Action& obj);
    void to_json(nlohmann::json& j, const Action& obj);

    /**
     * Override stream insertion operator to allow printing the Action class
     * 
     * @param os an output stream to insert into
     * @param a a reference to an action object
     * @returns the output stream referenced
     */
    std::ostream& operator<<(std::ostream& os, const Action& a);
}

#endif