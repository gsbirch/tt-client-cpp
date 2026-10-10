#pragma once

#include <tt/io/Message.h>
#include <tt/world/World.h>

#ifndef START_H
#define START_H

namespace tt {
    /**
     * The start message is sent from the server to a client
     * when a partner has been found and their
     * session begins.
     * 
     * @author Gage Birchmeier
     */
    class Start : public Message {
        public:
            /** The role the recipient of this message will have in the session */
            Role role;
            /** The story world in which the session will take place.
             * Note that this is an exposed unique_ptr. This is because the Client
             * will take ownership of the World object after it is received.
             * This pointer will be invalid after the Client moves its value.
             */
            std::unique_ptr<World> world;

            Start(): role(Role::NONE) {}

            std::string toString() const override;

            std::string type() const override {
                return "Start";
            }
    };

    void from_json(const nlohmann::json& j, Start& msg);
    void to_json(nlohmann::json& j, const Start& msg);
}

#endif