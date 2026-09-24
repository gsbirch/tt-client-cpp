#include <string>
#include <tt/world/Entity.h>
#include <tt/world/Signature.h>
#include <unordered_set>
#include <tt/Role.h>

#pragma once

#ifndef ACTION_H
#define ACTION_H

namespace tt {
    /**
     * An action is a {@link SignedAsset signed asset} that causes a change in a
     * {@link World story world's} {@link State state}. The {@link
     * edu.uky.cs.nil.tt.Agent agents} in a session negotiate when actions happen
     * via {@link Turn turns}.
     * 
     * @author Gage Birchmeier
     */
    class Action {
        public:
            const static std::string type;

            int id;
            std::string name;
            std::string description;
            /** The action's {@link Encoded code} */
            std::string code;
            // an action owns its own signature
            std::unique_ptr<Signature> signature;

            /**
             * Constructs a new action.
             * 
             * @param id the action's unique ID number
             * @param signature the action's unique signature
             * @param consenting an array of entities representing characters in the
             * the story world who need to agree to take the action
             * @param description the action's description
             * @param code the action's code
             */
            Action(int id, std::unique_ptr<Signature> signature, std::vector<const Entity *> consenting, std::string description, std::string code);

            // default constructor necessary for JSON deserialization
            Action(): signature(nullptr) {};

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
             * Returns true if the given story role controls one or more of the {@link
             * #getConsenting() consenting characters} for this action. An action with
             * no consenting character requires on the consent of the game master. An
             * action whose only consenting character is the player character requires
             * only the consent of the player. All other actions require the consent of
             * both roles.
             * 
             * @param role the role in question
             * @return true if the given role needs to consent to take this action
             */
            bool consents(Role role);

            std::string toString() const;

            friend std::ostream& operator<<(std::ostream& os, const Action& a);

            friend void from_json(const nlohmann::json& j, Action& obj);
            friend void to_json(nlohmann::json& j, const Action& obj);

        private:
            /**
             * Entities representing characters in the story world who need to agree
             * to take the action
             */
            std::vector<const Entity*> consenting;

            /** The action's {@link #consenting consenting characters} as a set */
            std::unordered_set<const Entity*> consentingSet;
    };

    void from_json(const nlohmann::json& j, Action& obj);
    void to_json(nlohmann::json& j, const Action& obj);
    std::ostream& operator<<(std::ostream& os, const Action& a);
}

#endif