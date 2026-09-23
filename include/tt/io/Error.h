#include <tt/io/Message.h>

#pragma once

#ifndef ERROR_H
#define ERROR_H

namespace tt {
    /**
     * The error message is sent from the {edu.uky.cs.nil.tt.Server server} to an
     * {@link edu.uky.cs.nil.tt.Agent agent} if one of the agent's messages causes
     * a problem or is incorrectly formatted.
     * 
     * @author Gage Birchmeier
     */
    class Error : public Message {
        public:
            /** A message explaining the error */
            std::string message;

            /**
             * Constructs a new error message.
             * 
             * @param message a string explaning the error
             */
            Error(const std::string& message);

            /**
             * Constructs a new error message using {@link Exception#getMessage() an
             * exception's message} as the message.
             * 
             * @param exception the exception that caused an error and whose message
             * should be send to the agent
             */
            Error(const std::exception& exception);

            Error() {}

            std::string toString() const override;

            void verify() const override;

            std::string type() const override {
                return "Error";
            }
    };

    void from_json(const nlohmann::json& j, Error& msg);
    void to_json(nlohmann::json& j, const Error& msg);
}

#endif