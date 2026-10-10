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
            Update() {};

            std::string toString() const override;

            /**
             * Returns the status object in this message.
             * 
             * @return the status included in this message
             */
            const Status *getStatus() const;

            std::string type() const override {
                return "Update";
            }

            friend void from_json(const nlohmann::json& j, Update& msg);
            friend void to_json(nlohmann::json& j, const Update& msg);
        private:
            /**
             * An object describing the history of the story so far, the current state
             * of the story world, and what turns are available for the agent to take,
             * if any
             */
            std::unique_ptr<Status> status;
    };

    void from_json(const nlohmann::json& j, Update& msg);
    void to_json(nlohmann::json& j, const Update& msg);
}

#endif