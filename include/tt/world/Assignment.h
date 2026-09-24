#include <string>
#include <tt/world/Variable.h>
#include <tt/world/Signature.h>

#pragma once

#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

namespace tt {
    
    class Assignment {
        public:
            /** The variable that is being assigned a value */
            const Variable* variable;

            /** The value assigned to the variable */
            Value value;

            /**
             * Whether or not this assignment can currently be observed. When an
             * assignment can be observed, it is known to be the case. When it cannot
             * be observed, it represents an assumed or last known value of the
             * variable.
             */
            bool visible;

            /** A natural language description of this assignments */
            std::string description;

            /** The {@link Encoded code} for this assignment */
            std::string code;

            Assignment(): variable(nullptr), value(nullptr) {};

            std::string toString() const;

            friend std::ostream& operator<<(std::ostream& os, const Assignment& a);
    };

    void from_json(const nlohmann::json& j, Assignment& obj);
    void to_json(nlohmann::json& j, const Assignment& obj);
    std::ostream& operator<<(std::ostream& os, const Assignment& a);
}

#endif