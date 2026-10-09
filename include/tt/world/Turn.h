#include <string>
#include <tt/Role.h>
#include <tt/world/Action.h>

#pragma once

#ifndef TURN_H
#define TURN_H

namespace tt {
    /**
     * A turn is one participant in a session
     * communicating their intentions for an {@link tt::Action action}
     * to the other participant.
     * <p>
     * There are four types of turns:
     * <ul>
     * <li>{@link Turn::Type#PROPOSE Propose}</li>
     * <li>{@link Turn::Type#SUCCEED Succeed}</li>
     * <li>{@link Turn::Type#FAIL Fail}</li>
     * <li>{@link Turn::Type#PASS Pass}</li>
     * </ul>
     * 
     * @author Gage Birchmeier
     */
    class Turn {
        public:
            /** The kind of turn being taken */
            enum Type {
                /**
                 * Used when one participant wants to suggest taking an action that
                 * requires the approval of their partner
                 */
                PROPOSE,
                
                /**
                 * Used when one participant agrees to take a proposed action, or when
                 * a participant can take an action that does not require approval
                 */
                SUCCEED,
                
                /**
                 * Used to reject a proposed action
                 */
                FAIL,
                
                /**
                 * Used when one participant does not want to act and wants to allow
                 * their partner to act instead
                 */
                PASS
            };

            /** The role of the participant taking this turn */
            Role role;
            /** The kind of turn being taken */
            Type type;

            Turn(): role(NONE), type(PASS), action(nullptr) {};

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
             * Returns a string of {@code 1}'s and {@code 0}'s that uniquely represents
             * this object among other objects of the same type in the same story world.
             * Every encoded object of the same type should return a string of the same
             * length, even if it must be padded with {@code 0}'s.
             * 
             * @return a string of 1's and 0's
             */
            const std::string& getCode() const;

            /**
             * Returns the action associated with this turn.
             * 
             * @return the action associated with this turn
             */
            const Action* getAction() const;

            friend std::ostream& operator<<(std::ostream& os, const Turn& a);

            friend void from_json(const nlohmann::json& j, Turn& obj);
            friend void to_json(nlohmann::json& j, const Turn& obj);

        private:
            /** The action that this turn is relevant to */
            std::unique_ptr<Action> action;

            /** A natural language description of this turn */
            std::string description;

            /** The Turn's encoding */
            std::string code;
    };

    void from_json(const nlohmann::json& j, Turn& obj);
    void to_json(nlohmann::json& j, const Turn& obj);
    std::ostream& operator<<(std::ostream& os, const Turn& a);

    std::string ttos(Turn::Type type);
    Turn::Type stot(std::string s);
}

#endif