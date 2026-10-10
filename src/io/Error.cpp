#include <tt/io/Error.h>

using namespace tt;

std::string tt::Error::toString() const
{
    return "[Error Message: \"" + message + "\"]";
}


void tt::from_json(const nlohmann::json &j, Error &msg)
{
    j.at("message").get_to(msg.message);
}

void tt::to_json(nlohmann::json &j, const Error &msg)
{
    j = {
        {"type", msg.type()},
        {"message", msg.message},
    };
}
