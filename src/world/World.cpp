#include <tt/world/World.h>
#include <tt/world/Registry.h>
#include <tt/world/Action.h>
#include <tt/util/JsonUtil.h>

using namespace tt;

std::string tt::World::toString() const
{
    std::string string = "[" + name + ":";
    string += " " + std::to_string(entities.size()) + " entities";
    string += "; " + std::to_string(variables.size()) + " variables";
    string += "; " + std::to_string(actions.size()) + " actions";
    string += "; " + std::to_string(endings.size()) + " endings";
    return string + "]";
}

std::ostream &tt::operator<<(std::ostream &os, const World &a)
{
    os << a.toString();
    return os;
}

void tt::from_json(const nlohmann::json &j, World &obj)
{
    j.at("name").get_to(obj.name);
    obj.entities = registerEntries<Entity>(j.at("entities"));
    obj.variables = registerEntries<Variable>(j.at("variables"));
    obj.actions = registerEntries<Action>(j.at("actions"));
    obj.endings = registerEntries<Ending>(j.at("endings"));
}

void tt::to_json(nlohmann::json &j, const World &obj)
{
    j = {
        {"name", obj.name},
        {"entities", obj.entities},
        {"variables", obj.variables},
        {"actions", obj.actions},
        {"endings", obj.endings},
    };
}
