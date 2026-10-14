#pragma once

#include <variant>
#include <cstdint>
#include <string>
#include <nlohmann/json.hpp>
#include <tt/world/Value.h>

#ifndef CONSTANT_H
#define CONSTANT_H


namespace tt {
    /**
     * A constant is a {@link tt::Value logical value} that always exists in all 
     * {@link tt::World story worlds}, such a the Boolean concepts of True and 
     * False, numbers, and so on.
     * 
     * @author Gage Birchmeier
     */
    class Constant : public Value {
        public:
            /** Type identifier used for deserialization */
            static constexpr std::string_view TYPE = "Constant";

            /** A constant representing nothing or the absence of a value */
            static const Constant NULL_CONSTANT;
            /** A constant representing Boolean true */
            static const Constant TRUE;
            /** A constant representing Boolean false */
            static const Constant FALSE;

            /** The value object associated with this constant */
            std::variant<std::monostate, bool, std::int64_t, double> value;

            /**
             * Constructs a new constant equivalent to {@link #NULL_CONSTANT}.
             * 
             * @param value a monostate, representing null
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

            std::string toString() const override;
            bool toBool() const override;
            double toNumber() const override;
            virtual std::string_view getType() const override;
        
        protected:
            virtual bool equals(const Value& other) const override;
    };

    void from_json(const nlohmann::json& j, Constant& obj);
    void to_json(nlohmann::json& j, const Constant& obj);
}

#endif