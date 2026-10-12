#pragma once

#include <string>

#ifndef VALUE_H
#define VALUE_H

namespace tt {
    // Forward declaration
    class Entity;

    /**
     * A value is a logical expression which evaluates to itself.
     * 
     * @author Gage Birchmeier
     */
    class Value {
        public:
            /**
             * If this value represents a Boolean, this method convert it into a bool;
             * all other values are converted to `false`.
             * 
             * @return a `bool`, or `false` if this value does not
             * represent a Boolean value
             */
            virtual bool toBool() const;

            /**
             * If this value represents a number, this method convert it into a Java
             * `double`; all other values are converted to `NaN`.
             * 
             * @return a `double`, or `NaN` if this value represents `NaN` or is
             * not a number
             */
            virtual double toNumber() const;

            /**
             * If this value represents an {@link tt::Entity}, this method returns a
             * pointer to that entity; all other values return nullptr.
             * 
             * @return a pointer to an entity, or `nullptr` if this value is not an entity
             */
            virtual const Entity * toEntity() const;

            /**
             * Returns a string representation of this object.
             * 
             * @returns a string describing this object
             */
            virtual std::string toString() const;

            /**
             * Returns the type of value (either "Entity" or "Constant").
             * Used in JSON deserialization
             * 
             * @return a string describing the type of value
             */
            virtual std::string_view getType() const;

            bool operator==(const Value& other) const;
            bool operator!=(const Value& other) const { return !(*this == other); }

            friend std::ostream& operator<<(std::ostream& os, const Value& obj);

        protected:
            /** Used for polymorphic ==
             * 
             * @param other another value object
             * @return whether the two values are equivalent
             */
            virtual bool equals(const Value& other) const = 0;
    };

    /**
     * Override stream insertion operator to allow printing the Value class
     * 
     * @param os an output stream to insert into
     * @param a a reference to an value object
     * @returns the output stream referenced
     */
    std::ostream& operator<<(std::ostream& os, const Value& obj);
}

#endif