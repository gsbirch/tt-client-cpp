#include <string>
#include <tt/Role.h>
#include <tt/io/Message.h>

#pragma once

#ifndef JOIN_H
#define JOIN_H

namespace tt {

    class Join: public Message {
        public:
            /** The new agent's name */
            std::string name;
            
            /**
             * The new agent's password, which must be provided if the agent is using a
             * {@link #name} that is reserved on this server, or which should be null if
             * the agent is not using a reserved name
             */
            std::string password;
            
            /**
             * The name of the story world this new agent wants to play in, or null if
             * they are willing to play in any story world
             */
            std::string world;
            
            /**
             * The role this new agent wants to have in their session, or null if they
             * are willing to play either role
             */
            Role role;
            
            /**
             * The name of the partner this new agent wants to play with, or null if
             * they are willing to play with any partner
             */
            std::string partner;

            /**
             * Constructs a new join message with an agent's credentials and
             * preferences.
             * 
             * @param name the new agent's name
             * @param password the new agent's password, or null if they are not using
             * a reserved name
             * @param world the world the new agent wants to play in, or null if they
             * have no preference
             * @param role the role the new agent wants to have, or null if they have no
             * preference
             * @param partner the name of the partner this new agent wants to play with,
             * or null if they have no preference
             */
            Join(std::string name, std::string password, std::string world, Role role, std::string partner);

            Join(): role(Role::NONE) {}

            std::string toString() const override;

            void verify() const override;

            /**
             * Returns true if this join request is compatible with another, meaning a
             * session could be created between the agents who sent the messages. This
             * method will not match two requests which both specify no preference for a
             * partner. In other words, at least one of the requests must specify a
             * partner.
             * 
             * @param other the join message of another agent waiting for a session
             * @return true if the join messages are compatible, or false if they are
             * not compatible
             */
            bool matches(Join other);

            std::string type() const override {
                return "Join";
            }

        private:

        template <typename T>
        static const bool matches(const T* o1, const T* o2) {
            return o1 == nullptr || o2 == nullptr || *o1 == *o2;
        }

        // should be unnecessary, as the Message class has this, and tostring is virtual??
        // friend std::ostream& operator<<(std::ostream& os, const Join& a);
    };

    void from_json(const nlohmann::json& j, Join& msg);
    void to_json(nlohmann::json& j, const Join& msg);

}

#endif