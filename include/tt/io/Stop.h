#include <tt/io/Message.h>

#pragma once

#ifndef STOP_H
#define STOP_H

namespace tt {
    class Stop : public Message {
        public:

            std::string toString() const override;

            void verify() const override;

            std::string type() const override {
                return "Error";
            }
    };

    void from_json(const nlohmann::json& j, Stop& msg);
    void to_json(nlohmann::json& j, const Stop& msg);
}

#endif