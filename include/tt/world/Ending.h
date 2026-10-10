#pragma once

#include <string>
#include <tt/world/Signature.h>
#include <memory>

#ifndef ENDING_H
#define ENDING_H

namespace tt {
    /**
     * An ending is a signed asset that represents one of
     * several possible ways a story can end.
     * 
     * @author Gage Birchmeier
     */
    class Ending {
        public:
            /** This asset's ID number, which is unique among other assets of the same type */
            int id;
            /** This asset's name, which is unique among other assets of the same type */
            std::string name;

            // default constructor for json deserialization
            Ending():id(0), signature(nullptr) {};

            /**
             * Returns a string representation of this object.
             * 
             * @returns a string describing this object
             */
            std::string toString() const;

            /**
             * Returns a human-readable natural language description of the object.
             * 
             * @return a human-readable natural language description of the object
             */
            const std::string& getDescription() const;

            /**
             * Returns a string of {@code 1}'s and {@code 0}'s that uniquely represents
             * this object among other objects of the same type in the same story world.
             * Every encoded object of the same type should return a string of the same
             * length, even if it must be padded with {@code 0}'s.
             * 
             * @return a string of 1's and 0's
             */
            const std::string& getCode() const;

            /**
             * Returns the signature of this object.
             * 
             * @return a pointer to the signature of this Action.
             */
            const Signature* getSignature() const;

            bool operator==(const Ending& rhs) const;

            friend void from_json(const nlohmann::json& j, Ending& obj);
            friend void to_json(nlohmann::json& j, const Ending& obj);
            friend std::ostream& operator<<(std::ostream& os, const Ending& a);

        private:
            /** The asset's unique signature */
            std::unique_ptr<Signature> signature;
            /** The ending's encoding */
            std::string code;
            /** A natural language description of this assignments */
            std::string description;
    };

    void from_json(const nlohmann::json& j, Ending& obj);
    void to_json(nlohmann::json& j, const Ending& obj);

    /**
     * Override stream insertion operator to allow printing the Ending class
     * 
     * @param os an output stream to insert into
     * @param a a reference to an ending object
     * @returns the output stream referenced
     */
    std::ostream& operator<<(std::ostream& os, const Ending& a);
}

#endif