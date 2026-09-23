#include <tt/io/Message.h>

#pragma once

#ifndef REPORT_H
#define REPORT_H

namespace tt {
    class Report : public Message {
        public:
            /** The name or ID of the survey question */
            std::string item;
            
            /** The answer the agent gives to the survey question */
            std::string value;
            
            /**
             * A comment provided by the agent explaining their answer, or null if the
             * question does not allow comments or the agent does not provide one
             */
            std::string comment;

            /**
             * Constructs a new report message.
             * 
             * @param item the name or ID of the survey question
             * @param value the answer the agent gives to the question
             * @param comment a comment explaining the answer, or null
             */
            Report(const std::string& item, const std::string& value, const std::string& comment);

            Report() {}

            std::string toString() const override;

            void verify() const override;

            std::string type() const override {
                return "Report";
            }
    };

    void from_json(const nlohmann::json& j, Report& msg);
    void to_json(nlohmann::json& j, const Report& msg);
}

#endif