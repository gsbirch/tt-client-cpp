#include <tt/Settings.h>

/** The name of this project */
const std::string tt::Settings::NAME = "Tandem Tales C++ Library";

/** The current version of this project */
const std::string tt::Settings::VERSION = "0.9.0";

/** The people who contributed significantly to this project */
const std::string tt::Settings::AUTHORS = "Gage Birchmeier";

/** A full title, including name, version number, and authors */
const std::string tt::Settings::TITLE = tt::Settings::NAME + " v" + tt::Settings::VERSION + " by " + tt::Settings::AUTHORS;

/** The maximum number of characters allowed in a {@link Named name} */
const int tt::Settings::NAME_MAX_LENGTH = 20;

/** The characters that can be legally used in a {@link Named name} */
const std::unordered_set<char> tt::Settings::NAME_ALLOWED_CHARACTERS;

/**
 * The amount of time in milliseconds between clock ticks.
 */
const long tt::Settings::TICK_SPEED = 5000; // Every 5 seconds

/**
 * The amount of time in milliseconds an agent has to send an expected
 * message before they will be disconnected for inactivity 
 */
const long tt::Settings::AGENT_TIMEOUT = 180000; // 3 minutes

/**
 * The number of passed turn in a row which causes a session to be
 * automatically stopped
 */
const int tt::Settings::PASS_LIMIT = 6;

/** The default port the server will listen on, if one is not specified */
const int tt::Settings::DEFAULT_PORT = 9005;

/**
 * The number of characters that can be read from a client socket before
 * an exception is thrown. This limit is used to avoid Out of Memory Errors
 * from malicious clients that send overly long inputs.
 */
const int tt::Settings::READ_LIMIT = 1024;