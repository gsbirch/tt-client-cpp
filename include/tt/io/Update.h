#include <tt/io/Message.h>

#pragma once

#ifndef UPDATE_H
#define UPDATE_H

namespace tt {
    class Update : public Message {
        public:

            std::string toString() const override;

            void verify() const override;

            std::string type() const override {
                return "Error";
            }
    };

    void from_json(const nlohmann::json& j, Update& msg);
    void to_json(nlohmann::json& j, const Update& msg);
}

#endif