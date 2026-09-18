#include <tt/Client.h>
#include <nlohmann/json.hpp>

#pragma once

#ifndef MESSAGE_H
#define MESSAGE_H

namespace tt {
    using json = nlohmann::json;

    class Message {
        public:
            // virtual function necessary for JSON serialization
            // Message must be polymorphic
            virtual ~Message() = default;
            /**
             * Configures a {@link GsonBuilder} to encode and decode {@link Message}
             * objects as JSON.
             * 
             * @param builder the GSON builder to configure
             */
            // static void configure(int builder);

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
            const Client* getClient() const;

            /**
             * Sets the {@link Agent} who sent this message. For messages sent to the
             * server, this method should be called soon after the message has been
             * parsed.
             * 
             * @param agent the agent who sent this message
             */
            void setClient(const Client* client);

            /**
             * Checks that this message is correctly configured and throws an exception
             * if not. This method should be called soon after the message has been
             * parsed. It should check that all the necessary fields are set and that
             * their values are legal values. 
             */
            virtual void verify() const;

            virtual std::string type() const = 0;

            virtual std::string toString() const;

            friend std::ostream& operator<<(std::ostream& os, const Message& a);

        private:
            /**
             * The agent who sent the message, if this message was sent to the server,
             * or null if this message was sent from the server.
             */
            // Agent agent;
            const Client* client;
    };
    
    
}

#endif