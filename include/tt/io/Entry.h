#include <string>
#include <nlohmann/json.hpp>

#pragma once

#ifndef ENTRY_H
#define ENTRY_H

namespace tt {
    /**
	 * A database entry represents an individual elements in the database,
	 * including its meta-data, such as its {@link #title title} and {@link
	 * #description description}.
	 * 
	 * @author Gage Birchmeier
	 */
    class Entry {
        public:
            /**
             * The unique system {@link Named name} for this element, which is
             * primarily used internally by the server
             */
            std::string name;
            /**
             * The title of this element that will be displayed to the participants
             * or potential participants of a session
             */
            std::string title;
            /**
             * A description of this element that will be shown to potential
             * participants of a session to help them decide if this elements is
             * something they are interested in
             */
            std::string description;

            Entry(const std::string& name, const std::string& title, const std::string& description):
            name(name), title(title), description(description) {}

            Entry(const std::string& name): name(name) {}

            Entry(const Entry& other):
            name(other.name), title(other.title), description(other.description) {}

            // default constructor for json deserialization
            Entry() {}

            std::string toString() const;

            friend std::ostream& operator<<(std::ostream& os, const Entry& a);
    };

    void from_json(const nlohmann::json& j, Entry& msg);
    void to_json(nlohmann::json& j, const Entry& msg);
}

#endif