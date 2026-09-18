#include <tt/io/End.h>

tt::End::End(const std::string& session):
session(session)
{
}

std::string tt::End::toString() const
{
    return "[End Message: session=\"" + session + "\"" + "]";
}

void tt::End::verify() const
{
}

void tt::from_json(const nlohmann::json &j, End &msg)
{
    j.at("session").get_to(msg.session);
}

void tt::to_json(nlohmann::json &j, const End &msg)
{
    j = {
        {"type", msg.type()},
        {"session", msg.session},
    };
}
