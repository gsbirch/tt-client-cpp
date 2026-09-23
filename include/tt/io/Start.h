#include <tt/io/Message.h>

#pragma once

#ifndef START_H
#define START_H

namespace tt {
    /**
     * The start message is sent from the {edu.uky.cs.nil.tt.Server server} to an
     * {@link edu.uky.cs.nil.tt.Agent agent} when a partner has been found and their
     * session begins.
     * 
     * @author Stephen G. Ware
     */
    class Start : public Message {
        public:
            std::string type;
            Role role;
            

            std::string toString() const override;

            void verify() const override;

            std::string type() const override {
                return "Error";
            }
    };

    void from_json(const nlohmann::json& j, Start& msg);
    void to_json(nlohmann::json& j, const Start& msg);
}

#endif