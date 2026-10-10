#pragma once

#include <tt/io/Message.h>
#include <string>

#ifndef END_H
#define END_H

namespace tt {
    /**
     * The end message is sent from the server to an
     * agent to indicate that the session had ended
     * and that they should now disconnect. If the server is logging sessions, this
     * message contains ID of the logged session.
     * 
     * @author Gage Birchmeier
     */
    class End : public Message {
        public:
            /** The ID of the session, or null if the session was not logged */
            std::string session;

            End() {};

            std::string toString() const override;

            std::string type() const override {
                return "End";
            }
    };

    void from_json(const nlohmann::json& j, End& msg);
    void to_json(nlohmann::json& j, const End& msg);
}

#endif