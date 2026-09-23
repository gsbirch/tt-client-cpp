#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

#pragma once

#ifndef REGISTRY_H
#define REGISTRY_H

namespace tt {
    // forward declarations
    class Action;
    class Assignment;
    class Ending;
    class Entity;
    class State;
    class Turn;
    class Variable;

    // The registry holds the unique pointers to each tt object
    // When a json object is deserialized, if it is equivalent to
    // something in the registry, instead a pointer is given to that registry item

    class Registry {
        public:
            static const Action* getAction(std::string code);
            static const Assignment* getAssignment(std::string code);
            static const Ending* getEnding(std::string code);
            static const Entity* getEntity(std::string code);
            static const State* getState(std::string code);
            static const Turn* getTurn(std::string code);
            static const Variable* getVariable(int id);

            static const Action* registerAction(std::unique_ptr<Action> action);
            static const Assignment* registerAssignment(std::unique_ptr<Assignment> assignment);
            static const Ending* registerEnding(std::unique_ptr<Ending> ending);
            static const Entity* registerEntity(std::unique_ptr<Entity> entity);
            static const State* registerState(std::unique_ptr<State> state);
            static const Turn* registerTurn(std::unique_ptr<Turn> turn);
            static const Variable* registerVariable(std::unique_ptr<Variable> variable);

        private:
            static std::unordered_map<std::string, std::unique_ptr<Action>> actions;
            static std::unordered_map<std::string, std::unique_ptr<Assignment>> assignments;
            static std::unordered_map<std::string, std::unique_ptr<Ending>> endings;
            static std::unordered_map<std::string, std::unique_ptr<Entity>> entities;
            static std::unordered_map<std::string, std::unique_ptr<State>> states;
            static std::unordered_map<std::string, std::unique_ptr<Turn>> turns;
            static std::vector<std::unique_ptr<Variable>> variables;
    };
}

#endif