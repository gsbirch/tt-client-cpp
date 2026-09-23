#include <tt/io/Entry.h>

std::string tt::Entry::toString() const
{
    return "[Entry: \"" + name + "\"]";
}

std::ostream &tt::operator<<(std::ostream &os, const Entry &a)
{
    os << a.toString();
    return os;
}

void tt::from_json(const nlohmann::json &j, Entry &msg)
{
    j.at("name").get_to(msg.name);
    j.at("title").get_to(msg.title);
    j.at("description").get_to(msg.description);
}

void tt::to_json(nlohmann::json &j, const Entry &msg)
{
    j = {
        {"name", msg.name},
        {"title", msg.title},
        {"description", msg.description}
    };
}
