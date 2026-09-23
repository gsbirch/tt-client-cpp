#include <tt/io/Connect.h>
#include <tt/io/Entry.h>

using namespace tt;

std::string tt::Connect::toString() const
{
    return "[Connect Message: " + std::to_string(worlds.size()) + " worlds; " + std::to_string(agents.size()) + " agents]";
}

void tt::Connect::verify() const
{
}

void tt::from_json(const nlohmann::json &j, Connect &msg)
{
    j.at("version").get_to(msg.version);
    j.at("worlds").get_to(msg.worlds);
    j.at("agents").get_to(msg.agents);
    j.at("available").get_to(msg.available);
}

void tt::to_json(nlohmann::json &j, const Connect &msg)
{
    j = {
        {"type", msg.type()},
        {"version", msg.version},
        {"worlds", msg.worlds},
        {"agents", msg.agents},
        {"available", msg.available},
    };
}

void tt::from_json(const nlohmann::json &j, Available &obj)
{
    j.at("agent").get_to(obj.agent);
    j.at("world").get_to(obj.world);
}

void tt::to_json(nlohmann::json &j, const Available &obj)
{
    j = {
        {"agent", obj.agent},
        {"world", obj.world},
    };
}
