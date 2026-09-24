#include <tt/util/JsonUtil.h>
#include <tt/world/Action.h>
#include <tt/world/Assignment.h>
#include <tt/world/Ending.h>
#include <tt/world/State.h>
#include <tt/world/Turn.h>
#include <tt/world/Variable.h>

namespace tt {
    template <typename T>
    const T* registerEntry(const nlohmann::json& j) {
        std::string code = j.at("code").get<std::string>();
        if constexpr (std::is_same_v<T, Action>) {
            if (const Action* e = Registry::getAction(code))
                return e;
            return Registry::registerAction(std::make_unique<Action>(j.get<Action>()));
        }
        if constexpr (std::is_same_v<T, Assignment>) {
            if (const Assignment* e = Registry::getAssignment(code)) {
                return e;
            }
            return Registry::registerAssignment(std::make_unique<Assignment>(j.get<Assignment>()));
        }
        if constexpr (std::is_same_v<T, Ending>) {
            if (const Ending* e = Registry::getEnding(code)) {
                return e;
            }
            return Registry::registerEnding(std::make_unique<Ending>(j.get<Ending>()));
        }
        if constexpr (std::is_same_v<T, Entity>) {
            if (const Entity* e = Registry::getEntity(code)) {
                return e;
            }
            return Registry::registerEntity(std::make_unique<Entity>(j.get<Entity>()));
        }
        if constexpr (std::is_same_v<T, State>) {
            if (const State* e = Registry::getState(code)) {
                return e;
            }
            return Registry::registerState(std::make_unique<State>(j.get<State>()));
        }
        if constexpr (std::is_same_v<T, Turn>) {
            if (const Turn* e = Registry::getTurn(code)) {
                return e;
            }
            return Registry::registerTurn(std::make_unique<Turn>(j.get<Turn>()));
        }
        if constexpr (std::is_same_v<T, Variable>) {
            int id = j.at("id").get<int>();
            if (const Variable* e = Registry::getVariable(id)) {
                return e;
            }
            return Registry::registerVariable(std::make_unique<Variable>(j.get<Variable>()));
        }
    }

    // Explicit instantiations: one for every type registerEntry<T>/registerEntries<T>
    // is actually called with elsewhere in the codebase. If you start calling
    // registerEntry<T> with a new type, add its instantiation here.
    template const Action* registerEntry<Action>(const nlohmann::json&);
    template const Assignment* registerEntry<Assignment>(const nlohmann::json&);
    template const Ending* registerEntry<Ending>(const nlohmann::json&);
    template const Entity* registerEntry<Entity>(const nlohmann::json&);
    template const State* registerEntry<State>(const nlohmann::json&);
    template const Turn* registerEntry<Turn>(const nlohmann::json&);
    template const Variable* registerEntry<Variable>(const nlohmann::json&);
}