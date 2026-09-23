#include <variant>
#include <cstdint>
#include <string>
#include <nlohmann/json.hpp>

#pragma once

#ifndef CONSTANT_H
#define CONSTANT_H

/**
 * A constant is a {@link Value logical value} that always exists in all {@link
 * World story worlds}, such a the Boolean concepts of True and False, numbers,
 * and so on.
 * 
 * @author Gage Birchmeier
 */
namespace tt {
    class Constant {
        public:
            // used for json
            static const std::string type;

            std::variant<std::monostate, bool, std::int64_t, float> value;

            /**
             * Constructs a new constant equivalent to {@link #NULL}.
             * 
             * @param value no value
             */
            Constant(std::monostate value): value(value) {};
            
            /**
             * Constructs a new constant that represents a Boolean value.
             * 
             * @param value the boolean value
             */
            Constant(bool value): value(value) {};

            /**
             * Constructs a new constant that represents an integer.
             * 
             * @param value the integer value
             */
            Constant(std::int16_t value): value(value) {};

            /**
             * Constructs a new constant that represents a decimal number.
             * 
             * @param value the decimal value
             */
            Constant(float value): value(value) {};

            // default constructor necessary for
            Constant(): value(std::monostate()) {};

            std::string toString() const;

            friend std::ostream& operator<<(std::ostream& os, const Constant& a);
    };

    void from_json(const nlohmann::json& j, Constant& obj);
    void to_json(nlohmann::json& j, const Constant& obj);
}

#endif