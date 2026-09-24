#include <tt/io/Message.h>

#pragma once

#ifndef CHOICE_H
#define CHOICE_H

namespace tt {
    /**
     * The choice message is sent from an {@link edu.uky.cs.nil.tt.Agent agent} to
     * the {edu.uky.cs.nil.tt.Server server} to signal what turn the agent wants
     * to take.
     * 
     * @author Gage Birchmeier
     */
    class Choice : public Message {
        public:
            /**
             * The index of a turn from the most recent {@link Update status update}
             * sent to the agent, starting at 0
             */
            int index;
            
            /**
             * Constructs a choice message for a given choice index.
             * 
             * @param index the index of the turn from the agent's most recent status
             * update that the agent wants to take
             */
            Choice(int index);

            Choice(): index(0) {}

            std::string toString() const override;

            void verify() const override;

            std::string type() const override {
                return "Choice";
            }
    };

    void from_json(const nlohmann::json& j, Choice& msg);
    void to_json(nlohmann::json& j, const Choice& msg);

}

#endif