#pragma once

#include <tt/io/Message.h>
#include <tt/world/Status.h>

#ifndef UPDATE_H
#define UPDATE_H

namespace tt {
    /**
     * The updates message is sent from the server to a client each time a turn is taken and the
     * state of the story world changes. An update is also sent immediately after
     * a session {@link tt::Start starts} to give the story world's initial state.
     * 
     * @author Gage Birchmeier
     */
    class Update : public Message {
        public:
            /**
             * An object describing the history of the story so far, the current state
             * of the story world, and what turns are available for the agent to take,
             * if any.
             * Note that this is an exposed unique_ptr. This is because the {@link tt::Client}
             * will take ownership of the Status object after it is received.
             * This pointer will be invalid after the Client moves its value.
             */
            std::unique_ptr<Status> status;

            Update() {};

            std::string toString() const override;

            std::string type() const override {
                return "Update";
            }

            friend void from_json(const nlohmann::json& j, Update& msg);
            friend void to_json(nlohmann::json& j, const Update& msg);
        private:
            
    };

    void from_json(const nlohmann::json& j, Update& msg);
    void to_json(nlohmann::json& j, const Update& msg);
}

#endif