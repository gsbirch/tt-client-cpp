#include "../Agent.h"

#pragma once

#ifndef MESSAGE_H
#define MESSAGE_H

namespace tt {
    class Message {
        public:
            /**
             * Configures a {@link GsonBuilder} to encode and decode {@link Message}
             * objects as JSON.
             * 
             * @param builder the GSON builder to configure
             */
            static void configure(int builder);

            /**
             * Constructs a message.
             */
            Message();

            /**
             * Returns the {@link Agent} who sent this message, or null if this message
             * was sent by the server.
             * 
             * @return the agent who sent the message, or null
             */
            Agent getAgent();

            /**
             * Sets the {@link Agent} who sent this message. For messages sent to the
             * server, this method should be called soon after the message has been
             * parsed.
             * 
             * @param agent the agent who sent this message
             */
            void setAgent(Agent agent);

            /**
             * Checks that this message is correctly configured and throws an exception
             * if not. This method should be called soon after the message has been
             * parsed. It should check that all the necessary fields are set and that
             * their values are legal values. 
             */
            void verify();

        private:
            /**
             * The agent who sent the message, if this message was sent to the server,
             * or null if this message was sent from the server.
             */
            Agent agent;
    };

}

#endif