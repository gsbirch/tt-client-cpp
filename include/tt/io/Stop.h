#pragma once

#include <tt/io/Message.h>
#include <tt/world/Ending.h>
#include <tt/Role.h>

#ifndef STOP_H
#define STOP_H

namespace tt {
    /**
     * The stop message can be sent from either the server or the client to indicate that no more
     * turns will happen in the session (though {@link tt::Report reports} may still
     * happen) and that the session should end soon.
     * <p>
     * An agent can send the stop message (1) if they want to stop a session early
     * before the story had reached one of its defined endings, or (2) to indicate
     * they are finished and have no more {@link tt::Report reports} to send. After an
     * agent sends the stop message, they may not send any more messages (including
     * reports), though they may stay connected to wait for the {@link tt::End end}
     * message.
     * <p>
     * The server will send the stop message to notify both agents that no more
     * turns can be taken. This will happen when (1) the story reaches one of its
     * defined endings, (2) one of the agents in the session stops it early, (3) one
     * of the agents in the session disconnects, or (4) the server shuts down.
     * <p>
     * Once an agent receives the stop message, it cannot take any more turns, even
     * if the story has not reached one of its defined endings. However, an agent
     * may still send {@link tt::Report reports}. When an agent is done sending reports,
     * it should send a stop message to indicate this.
     * <p>
     * After both agents have either sent stop messages or disconnected, the 
     * {@link tt::End end} message will be sent to both agents.
     * 
     * @author Gage Birchmeier
     */
    class Stop : public Message {
        public:
            /**
             * The participant who ended the session, or null if the session reached a
             * pre-defined ending or was ended by the server
             */
            Role role;
            /** A message explaining how the session ended */
            std::string message;

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

            Stop();

            std::string toString() const override;

            /**
             * Returns the ending object sent in this message,
             * or nullptr if there isn't one.
             * 
             * @return the ending included in this message
             */
            const Ending *getEnding() const;

            std::string type() const override {
                return "Stop";
            }

            friend void from_json(const nlohmann::json& j, Stop& msg);
            friend void to_json(nlohmann::json& j, const Stop& msg);

        private:
            /**
             * One of the {@link tt::World#getEndings() pre-defined endings} 
             * from the story world, or null if this session did not reach one
             * of those endings
             */
            std::unique_ptr<Ending> ending;
    };

    void from_json(const nlohmann::json& j, Stop& msg);
    void to_json(nlohmann::json& j, const Stop& msg);
}

#endif