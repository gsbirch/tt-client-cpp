#include <tt/io/Message.h>
#include <tt/world/Status.h>

#pragma once

#ifndef UPDATE_H
#define UPDATE_H

namespace tt {
    /**
     * The updates message is sent from the {edu.uky.cs.nil.tt.Server server} to an
     * {@link edu.uky.cs.nil.tt.Agent agent} each time a turn is taken and the
     * state of the story world changes. An update is also sent immediately after
     * a session {@link Start starts} to give the story world's initial state.
     * 
     * @author Stephen G. Ware
     */
    class Update : public Message {
        public:
            /**
             * An object describing the history of the story so far, the current state
             * of the story world, and what turns are available for the agent to take,
             * if any
             */
            Status status;

            Update() {};

            std::string toString() const override;

            void verify() const override;

            std::string type() const override {
                return "Update";
            }
    };

    void from_json(const nlohmann::json& j, Update& msg);
    void to_json(nlohmann::json& j, const Update& msg);
}

#endif