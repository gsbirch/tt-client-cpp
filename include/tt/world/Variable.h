#include <string>
#include <memory>
#include <tt/world/Signature.h>

#pragma once

#ifndef VARIABLE_H
#define VARIABLE_H

namespace tt {
    class Variable {
        public:
            std::string name;
            int id;
            // A variable owns its own signature
            std::unique_ptr<Signature> signature;

            /**
             * The name of the {@link Encoding encoding} this variable uses to encode
             * its values
             */
            std::string encoding;
            std::string description;

            Variable();

            std::string toString() const;

            friend std::ostream& operator<<(std::ostream& os, const Variable& a);
    };

    void from_json(const nlohmann::json& j, Variable& obj);
    void to_json(nlohmann::json& j, const Variable& obj);
    std::ostream& operator<<(std::ostream& os, const Variable& a);
}

#endif