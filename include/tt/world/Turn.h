#include <string>
#include <tt/Role.h>
#include <tt/world/Action.h>

#pragma once

#ifndef TURN_H
#define TURN_H

namespace tt {
    /**
     * A turn is one participant in a {@link  edu.uky.cs.nil.tt.Session session}
     * communicating their intentions for an {@link Action action} to the other
     * participant.
     * <p>
     * There are four types of turns:
     * <ul>
     * <li>{@link Type#PROPOSE Propose}</li>
     * <li>{@link Type#SUCCEED Succeed}</li>
     * <li>{@link Type#FAIL Fail}</li>
     * <li>{@link Type#PASS Pass}</li>
     * </ul>
     * 
     * @author Stephen G. Ware
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
            
            /** The action that this turn is relevant to */
            const Action * action;

            /** A natural language description of this turn */
            std::string description;

            std::string code;

            Turn(): role(NONE), type(PASS), action(nullptr) {};

            std::string toString() const;

            friend std::ostream& operator<<(std::ostream& os, const Turn& a);

            friend void from_json(const nlohmann::json& j, Turn& obj);
            friend void to_json(nlohmann::json& j, const Turn& obj);

        private:
            
    };

    void from_json(const nlohmann::json& j, Turn& obj);
    void to_json(nlohmann::json& j, const Turn& obj);
    std::ostream& operator<<(std::ostream& os, const Turn& a);

    std::string ttos(Turn::Type type);
    Turn::Type stot(std::string s);
}

#endif