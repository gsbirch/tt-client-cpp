#include <tt/world/World.h>8
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
    fromJsonPtrVec(j, "entities", obj.entities);
    fromJsonPtrVec(j, "variables", obj.variables);
    fromJsonPtrVec(j, "actions", obj.actions);
    fromJsonPtrVec(j, "endings", obj.endings);
}

void tt::to_json(nlohmann::json &j, const World &obj)
{
    j = {
        {"name", obj.name},
    };
    toJsonInVector(j, "entities", obj.entities);
    toJsonInVector(j, "variables", obj.variables);
    toJsonInVector(j, "actions", obj.actions);
    toJsonInVector(j, "endings", obj.endings);
}
