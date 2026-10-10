#pragma once

#include <variant>
#include <cstdint>
#include <string>
#include <nlohmann/json.hpp>

#ifndef CONSTANT_H
#define CONSTANT_H


namespace tt {
    /**
     * A constant is a {@link Value logical value} that always exists in all 
     * {@link tt::World story worlds}, such a the Boolean concepts of True and 
     * False, numbers, and so on.
     * 
     * @author Gage Birchmeier
     */
    class Constant {
        public:
            /** Type used for JSON deserialization */
            static const std::string type;

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

            /**
             * Returns a string representation of this object.
             * 
             * @returns a string describing this object
             */
            std::string toString() const;

            /**
             * Returns whether the value is a boolean equal to true
             * 
             * @return true if the value is equivalent to the bool true, false otherwise
             */
            bool toBool() const;

            /**
             * Returns the numeric value of this constant
             * 
             * @return the value converted to a double
             */
            double toNumber() const;

            bool operator==(const Constant& rhs) const;

            friend std::ostream& operator<<(std::ostream& os, const Constant& a);
    };

    void from_json(const nlohmann::json& j, Constant& obj);
    void to_json(nlohmann::json& j, const Constant& obj);

    /**
     * Override stream insertion operator to allow printing the Constant class
     * 
     * @param os an output stream to insert into
     * @param a a reference to an constant object
     * @returns the output stream referenced
     */
    std::ostream& operator<<(std::ostream& os, const Constant& a);
}

#endif