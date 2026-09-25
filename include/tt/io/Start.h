#include <tt/io/Message.h>
#include <tt/world/World.h>

#pragma once

#ifndef START_H
#define START_H

namespace tt {
    /**
     * The start message is sent from the {edu.uky.cs.nil.tt.Server server} to an
     * {@link edu.uky.cs.nil.tt.Agent agent} when a partner has been found and their
     * session begins.
     * 
     * @author Stephen G. Ware
     */
    class Start : public Message {
        public:
            /** The role the recipient of this message will have in the session */
            Role role;
            /** The story world in which the session will take place */
            std::unique_ptr<World> world;

            Start(): role(Role::NONE) {}

            std::string toString() const override;

            void verify() const override;

            std::string type() const override {
                return "Start";
            }
    };

    void from_json(const nlohmann::json& j, Start& msg);
    void to_json(nlohmann::json& j, const Start& msg);
}

#endif