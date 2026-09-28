#include <tt/world/Status.h>
#include <tt/util/JsonUtil.h>
#include <tt/world/Ending.h>

using namespace tt;

std::string tt::Status::toString() const
{
    std::string string = "[" + rtos(role) + " status:";
    string += " " + std::to_string(history.size()) + " turns";
    string += "; " + std::to_string(choices.size()) + " choices";
    return string + "]";
}

const std::vector<const Turn *>& tt::Status::getHistory() const
{
    if (historyPtrs.size() != history.size()) {
        historyPtrs.clear();
        historyPtrs.reserve(history.size());
        for (const auto& p : history) {
            historyPtrs.push_back(p.get());
        }
    }
    return historyPtrs;
}

const std::vector<const Entity *> tt::Status::getDescriptions() const
{
    if (descriptionsPtrs.size() != descriptions.size()) {
        descriptionsPtrs.clear();
        descriptionsPtrs.reserve(descriptions.size());
        for (const auto& p : descriptions) {
            descriptionsPtrs.push_back(p.get());
        }
    }
    return descriptionsPtrs;
}

const std::vector<const Turn *> tt::Status::getChoices() const
{
    if (choicesPtrs.size() != choices.size()) {
        choicesPtrs.clear();
        choicesPtrs.reserve(choices.size());
        for (const auto& p : choices) {
            choicesPtrs.push_back(p.get());
        }
    }
    return choicesPtrs;
}

const State *tt::Status::getState() const
{
    return state.get();
}

const Ending *tt::Status::getEnding() const
{
    return ending.get();
}

std::ostream &tt::operator<<(std::ostream &os, const Status &a)
{
    os << a.toString();
    return os;
}

void tt::from_json(const nlohmann::json &j, Status &obj)
{
    std::string r = j.at("role").get<std::string>();
    obj.role = stor(r);
    fromJsonPtrVec(j, "history", obj.history);
    fromJsonPtr(j, "state", obj.state);
    fromJsonPtrVec(j, "descriptions", obj.descriptions);
    if (j.contains("choices") && !j["choices"].is_null()) {
        fromJsonPtrVec(j, "choices", obj.choices);
    }
    if (j.contains("ending") && !j["ending"].is_null()) {
        fromJsonPtr(j, "ending", obj.ending);
    }
}

void tt::to_json(nlohmann::json &j, const Status &obj)
{
    j = {
        {"role", rtos(obj.role)},
        {"state", *obj.state},
        {"ending", *obj.ending},
    };
    toJsonInVector(j, "history", obj.history);
    toJsonInVector(j, "descriptions", obj.descriptions);
    toJsonInVector(j, "choices", obj.choices);
}
