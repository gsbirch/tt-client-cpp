#include <tt/io/Message.h>
#include <tt/world/Ending.h>

#pragma once

#ifndef STOP_H
#define STOP_H

namespace tt {
    class Stop : public Message {
        public:
            /**
             * One of the {@link edu.uky.cs.nil.tt.world.World#getEndings() pre-defined
             * endings} from the story world, or null if this session did not reach one
             * of those endings
             */
            const Ending * ending;
            /**
             * The participant who ended the session, or null if the session reached a
             * pre-defined ending or was ended by the server
             */
            Role role;
            /** A message explaining how the session ended */
            std::string message;

            /**
             * Constructs a stop message from a pre-defined story ending.
             * 
             * @param ending the pre-defined ending that occurred in the story
             */
            Stop(const Ending *ending);
            /**
             * Constructs a stop message for the role who stopped the session before
             * it reached a pre-defined ending.
             * 
             * @param role the role who stopped the story
             */
            Stop(Role role);
            /**
             * Constructs a stop message with a string explaining how the session
             * ended before it reached a pre-definded ending.
             * 
             * @param message a string explaining why the session ended
             */
            Stop(const std::string& message);

            // default constructor for json deserialization
            Stop();

            std::string toString() const override;

            void verify() const override;

            std::string type() const override {
                return "Stop";
            }
    };

    void from_json(const nlohmann::json& j, Stop& msg);
    void to_json(nlohmann::json& j, const Stop& msg);
}

#endif