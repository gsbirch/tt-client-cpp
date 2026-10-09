#include <tt/world/World.h>
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

const std::vector<const Entity *>& tt::World::getEntities() const
{
    if (entitiesPtrs.size() != entities.size()) {
        entitiesPtrs.clear();
        entitiesPtrs.reserve(entities.size());
        for (const auto& p : entities) {
            entitiesPtrs.push_back(p.get());
        }
    }
    return entitiesPtrs;
}

const std::vector<const Variable *> &tt::World::getVariables() const
{
    if (variablesPtrs.size() != variables.size()) {
        variablesPtrs.clear();
        variablesPtrs.reserve(variables.size());
        for (const auto& p : variables) {
            variablesPtrs.push_back(p.get());
        }
    }
    return variablesPtrs;
}

const std::vector<const Action *> &tt::World::getActions() const
{
    if (actionsPtrs.size() != actions.size()) {
        actionsPtrs.clear();
        actionsPtrs.reserve(actions.size());
        for (const auto& p : actions) {
            actionsPtrs.push_back(p.get());
        }
    }
    return actionsPtrs;
}

const std::vector<const Ending *> &tt::World::getEndings() const
{
    if (endingsPtrs.size() != endings.size()) {
        endingsPtrs.clear();
        endingsPtrs.reserve(endings.size());
        for (const auto& p : endings) {
            endingsPtrs.push_back(p.get());
        }
    }
    return endingsPtrs;
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
