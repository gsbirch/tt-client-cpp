#include <tt/io/Message.h>
#include <string>

#pragma once 

#ifndef END_H
#define END_H

namespace tt {
    /**
     * The end message is sent from the {edu.uky.cs.nil.tt.Server server} to an
     * {@link edu.uky.cs.nil.tt.Agent agent} to indicate that the session had ended
     * and that they should now disconnect. If the server is logging sessions, this
     * message contains ID of the logged session.
     * 
     * @author Gage Birchmeier
     */
    class End : public Message {
        public:
            /** The ID of the session, or null if the session was not logged */
            std::string session;

            /**
             * Constructs a new end message with the given session ID.
             * 
             * @param session the ID of the session that ended, or null if the session
             * was not logged
             */
            End(const std::string& session);

            std::string toString() const override;

            void verify() const override;

            std::string type() const override {
                return "End";
            }
    };

    void from_json(const nlohmann::json& j, End& msg);
    void to_json(nlohmann::json& j, const End& msg);
}

#endif