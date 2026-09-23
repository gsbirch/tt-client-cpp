#include <tt/world/Registry.h>
#include <tt/world/Action.h>
#include <tt/world/Assignment.h>
#include <tt/world/Ending.h>
#include <tt/world/Entity.h>
#include <tt/world/State.h>
#include <tt/world/Turn.h>
#include <tt/world/Variable.h>

using namespace tt;

const Action *tt::Registry::getAction(std::string code)
{
    auto it = actions.find(code);
    if (it == actions.end()) return nullptr;
    return it->second.get();
}

const Assignment *tt::Registry::getAssignment(std::string code)
{
    auto it = assignments.find(code);
    if (it == assignments.end()) return nullptr;
    return it->second.get();
}

const Ending *tt::Registry::getEnding(std::string code)
{
    auto it = endings.find(code);
    if (it == endings.end()) return nullptr;
    return it->second.get();
}

const Entity *tt::Registry::getEntity(std::string code)
{
    auto it = entities.find(code);
    if (it == entities.end()) return nullptr;
    return it->second.get();
}

const State *tt::Registry::getState(std::string code)
{
    auto it = states.find(code);
    if (it == states.end()) return nullptr;
    return it->second.get();
}

const Turn *tt::Registry::getTurn(std::string code)
{
    auto it = turns.find(code);
    if (it == turns.end()) return nullptr;
    return it->second.get();
}

const Variable *tt::Registry::getVariable(int id)
{
    if (id < variables.size() || variables[id] == nullptr) return nullptr;
    return variables[id].get();
}

const Action *tt::Registry::registerAction(std::unique_ptr<Action> action)
{
    std::string code = action->code;
    actions.insert({code, std::move(action)});
    return getAction(code);
}

const Assignment *tt::Registry::registerAssignment(std::unique_ptr<Assignment> assignment)
{
    std::string code = assignment->code;
    assignments.insert({code, std::move(assignment)});
    return getAssignment(code);
}

const Ending *tt::Registry::registerEnding(std::unique_ptr<Ending> ending)
{
    std::string code = ending->code;
    endings.insert({code, std::move(ending)});
    return getEnding(code);
}

const Entity *tt::Registry::registerEntity(std::unique_ptr<Entity> entity)
{
    std::string code = entity->code;
    entities.insert({code, std::move(entity)});
    return getEntity(code);
}

const State *tt::Registry::registerState(std::unique_ptr<State> state)
{
    std::string code = state->code;
    states.insert({code, std::move(state)});
    return getState(code);
}

const Turn *tt::Registry::registerTurn(std::unique_ptr<Turn> turn)
{
    std::string code = turn->code;
    turns.insert({code, std::move(turn)});
    return getTurn(code);
}

const Variable *tt::Registry::registerVariable(std::unique_ptr<Variable> variable)
{
    int idx = variable->id;
    while (idx > variables.size()) {
        variables.push_back(nullptr);
    }
    variables[idx] = std::move(variable);
    return getVariable(idx);
}
