#pragma once

#include <tt/io/Message.h>

#ifndef ERROR_H
#define ERROR_H

namespace tt {
    /**
     * The error message is sent from the server to an
     * agent if one of the agent's messages causes
     * a problem or is incorrectly formatted.
     * 
     * @author Gage Birchmeier
     */
    class Error : public Message {
        public:
            /** A message explaining the error */
            std::string message;

            Error() {}

            std::string toString() const override;

            std::string type() const override {
                return "Error";
            }
    };

    void from_json(const nlohmann::json& j, Error& msg);
    void to_json(nlohmann::json& j, const Error& msg);
}

#endif