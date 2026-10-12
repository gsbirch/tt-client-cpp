#pragma once

#include <variant>
#include <tt/world/Constant.h>
#include <tt/world/Entity.h>
#include <tt/util/JsonUtil.h>

#ifndef SIGNATURE_H
#define SIGNATURE_H

namespace tt {
    /**
     * A signature is a logical object that identifies a
     * parameterized asset in a {@link tt::World story world}. A signature
     * is defined by a {@link #name name} and an ordered sequence of 0 to many
     * values, called its arguments. Signatures make it possible to
     * tell how different assets of the same type are similar and different from one
     * another. 
     * 
     * @author Gage Birchmeier
     */
    class Signature {
        public:
        /** The signature's name */
	    std::string name;

        Signature() {};

        bool operator==(const Signature& s);

        /**
         * Returns an unmodifiable list of all arguments defined in this story
         * world ordered by the ID numbers.
         * 
         * @return a vector of arguments defined in this story world
         */
        const std::vector<const Value *>& getArguments() const;

        /**
             * Returns a string representation of this object.
             * 
             * @returns a string describing this object
             */
        std::string toString() const;

        friend std::ostream& operator<<(std::ostream& os, const Signature& a);
        friend void from_json(const nlohmann::json& j, Signature& obj);
        friend void to_json(nlohmann::json& j, const Signature& obj);

        private:
            /** The signature's arguments */
            std::vector<std::unique_ptr<Value>> arguments;
            /** Cache of argument ptrs */
            mutable std::vector<const Value *> argumentsPtrs;
    };

    void from_json(const nlohmann::json& j, Signature& obj);
    void to_json(nlohmann::json& j, const Signature& obj);

    /**
     * Override stream insertion operator to allow printing the Signature class
     * 
     * @param os an output stream to insert into
     * @param a a reference to an signature object
     * @returns the output stream referenced
     */
    std::ostream& operator<<(std::ostream& os, const Signature& a);
}

#endif