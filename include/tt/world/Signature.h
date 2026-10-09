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
	
	    /** The signature's arguments */
	    std::vector<Value> arguments;

        /**
         * Constructs a new signature from a name and an array of arguments.
         * 
         * @param name the signature's name
         * @param arguments an ordered sequence of 0 to many values 
         */
        // Signature(std::string name, std::vector<Value> arguments);

        Signature();

        bool operator==(const Signature& s);

        std::string toString() const;

        friend std::ostream& operator<<(std::ostream& os, const Signature& a);
    };

    void from_json(const nlohmann::json& j, Signature& obj);
    void to_json(nlohmann::json& j, const Signature& obj);
    std::ostream& operator<<(std::ostream& os, const Signature& a);
}

#endif