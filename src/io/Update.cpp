#include <tt/io/Update.h>

std::string tt::Update::toString() const
{
    return "[Update Message: " + std::to_string(status.choices.size()) + " choices]";
}

void tt::Update::verify() const
{
}

void tt::from_json(const nlohmann::json &j, Update &msg)
{
    j.at("status").get_to(msg.status);
}

void tt::to_json(nlohmann::json &j, const Update &msg)
{
    j = {
        {"type", msg.type()},
        {"status", msg.status},
    };
}
