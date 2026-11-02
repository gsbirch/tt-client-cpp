#include <tt/Settings.h>

const std::string tt::Settings::NAME = "Tandem Tales C++ Library";

const std::string tt::Settings::VERSION = "0.9.0";

const std::string tt::Settings::SERVER_VERSION = "1.0.0";

const std::string tt::Settings::AUTHORS = "Gage Birchmeier";

const std::string tt::Settings::TITLE = tt::Settings::NAME + " v" + tt::Settings::VERSION + " by " + tt::Settings::AUTHORS;

const int tt::Settings::NAME_MAX_LENGTH = 20;

/** The default port the server will listen on, if one is not specified */
const int tt::Settings::DEFAULT_PORT = 9005;
