#include <tt/util/JsonUtil.h>

#pragma once

#ifndef UTIL_H
#define UTIL_H

// this is a space for any utility functions I need
namespace tt {
    /**
     * A function to convert a Value object into a string.
     * 
     * @param v A Value object
     * @returns A string representation of v
     */
    inline std::string vtos(const Value &v) {
        if (auto c = std::get_if<std::unique_ptr<tt::Constant>>(&v)) return (*c)->toString();
        if (auto e = std::get_if<std::unique_ptr<tt::Entity>>(&v))  return (*e)->toString();
        return "Value of unknown type";
    }

    /**
	 * Throws an exception if the given string is empty.
	 * 
	 * @param s the string which should not be empty
	 * @param description a short description of the type of string, used in the
	 * message of the exception which is thrown if the string is empty
	 * @throws std::invalid_argument if the string given is empty
	 */
    inline void requireNonEmpty(const std::string& s, const std::string& description) {
        if (s.empty())
            throw std::invalid_argument(description + " cannot be empty");
    }

    /**
	 * Throws an exception if the given number is less than 0.
	 * 
	 * @param s the string which should not be non-negative
	 * @param description a short description of the number, used in the
	 * message of the exception which is thrown if the string is empty
	 * @throws std::invalid_argument if the number is negative
	 */
    template <typename T>
    inline void requireNonNegative(const T& x, const std::string& description) {
        if (x < 0)
            throw std::invalid_argument(description + " cannot be empty");
    }
}


#endif
