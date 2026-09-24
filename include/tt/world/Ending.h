#include <string>
#include <tt/world/Signature.h>
#include <memory>

#pragma once

#ifndef ENDING_H
#define ENDING_H

namespace tt {
    class Ending {
        public:
            
            int id;

            std::string name;

            std::unique_ptr<Signature> signature;

            /** The action's {@link Encoded code} */
            std::string code;

            std::string description;

            // default constructor for json deserialization
            Ending(): signature(nullptr) {};

            std::string toString() const;

            friend std::ostream& operator<<(std::ostream& os, const Ending& a);
    };

    void from_json(const nlohmann::json& j, Ending& obj);
    void to_json(nlohmann::json& j, const Ending& obj);
}

#endif