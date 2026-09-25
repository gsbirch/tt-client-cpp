#include <string>
#include <unordered_set>

#pragma once

#ifndef SETTINGS_H
#define SETTINGS_H

namespace tt {
    class Settings {
        public:
            /** The name of this project */
            static const std::string NAME;

            /** The current version of this project */
            static const std::string VERSION;

            /** The people who contributed significantly to this project */
            static const std::string AUTHORS;

            /** A full title, including name, version number, and authors */
            static const std::string TITLE;

            /** The maximum number of characters allowed in a {@link Named name} */
            static const int NAME_MAX_LENGTH;

            /** The characters that can be legally used in a {@link Named name} */
            static const std::unordered_set<char> NAME_ALLOWED_CHARACTERS;

            /**
             * The amount of time in milliseconds between clock ticks.
             */
            static const long TICK_SPEED;

            /**
             * The amount of time in milliseconds an agent has to send an expected
             * message before they will be disconnected for inactivity 
             */
            static const long AGENT_TIMEOUT;

            /**
             * The number of passed turn in a row which causes a session to be
             * automatically stopped
             */
            static const int PASS_LIMIT;

            /** The default port the server will listen on, if one is not specified */
            static const int DEFAULT_PORT;

            /**
             * The number of characters that can be read from a client socket before
             * an exception is thrown. This limit is used to avoid Out of Memory Errors
             * from malicious clients that send overly long inputs.
             */
            static const int READ_LIMIT;
    };
}

#endif