#include <string>
#include <unordered_set>

#pragma once

#ifndef SETTINGS_H
#define SETTINGS_H

namespace tt {
    /**
     * Constants and other project-wide settings.
     * 
     * @author Gage Birchmeier
     */
    class Settings {
        public:
            /** The name of this project */
            static const std::string NAME;

            /** The current version of this project */
            static const std::string VERSION;

            /** The server version number this project expects */
            static const std::string SERVER_VERSION;

            /** The people who contributed significantly to this project */
            static const std::string AUTHORS;

            /** A full title, including name, version number, and authors */
            static const std::string TITLE;

            /** The maximum number of characters allowed in a name */
            static const int NAME_MAX_LENGTH;

            /** The default port the server will listen on, if one is not specified */
            static const int DEFAULT_PORT;

    };
}

#endif