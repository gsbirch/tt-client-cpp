#pragma once

#include <nlohmann/json.hpp>

#ifndef MESSAGE_H
#define MESSAGE_H

namespace tt {

    // forward declaration
    class Client;

    /** Alias for nlohmann's json */
    using json = nlohmann::json;

    /**
     * A message is information sent to or from a server in JSON format.
     * 
     * @author Gage Birchmeier
     */
    class Message {
        public:
            // virtual function necessary for JSON serialization
            // Message must be polymorphic
            virtual ~Message() = default;

            /**
             * Constructs a message.
             */
            Message(): client(nullptr) {};

            /**
             * Returns the {@link tt::Client} who sent this message, or null if this message
             * was sent by the server.
             * 
             * @return the agent who sent the message, or null
             */
            const Client* getClient() const;

            /**
             * Sets the {@link tt::Client} who sent this message. For messages sent to the
             * server, this method should be called soon after the message has been
             * parsed.
             * 
             * @param client the agent who sent this message
             */
            void setClient(const Client* client);

            /**
             * Type used for JSON deserialization
             */
            virtual std::string type() const = 0;

            /**
             * Returns a string representation of this object.
             * 
             * @returns a string describing this object
             */
            virtual std::string toString() const;

            friend std::ostream& operator<<(std::ostream& os, const Message& a);

        private:
            /**
             * The agent who sent the message, if this message was sent to the server,
             * or null if this message was sent from the server.
             */
            const Client* client;
    };
    /**
     * Override stream insertion operator to allow printing the Action class
     * 
     * @param os an output stream to insert into
     * @param a a reference to an action object
     * @returns the output stream referenced
     */
    std::ostream& operator<<(std::ostream& os, const Message& a);
}

#endif