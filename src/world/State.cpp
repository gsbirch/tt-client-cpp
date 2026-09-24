#include <tt/world/State.h>
#include <tt/world/Assignment.h>
#include <tt/util/JsonUtil.h>

std::string tt::State::toString() const
{
    std::string string = "[";
    for (int i = 0; i < assignments.size(); i++) {
        const Assignment * a = assignments[i];
        string += a->toString();
    }
    return string;
}

std::ostream &tt::operator<<(std::ostream &os, const State &a)
{
    os << a.toString();
    return os;
}

void tt::from_json(const nlohmann::json &j, State &obj)
{
    obj.assignments = registerEntries<Assignment>(j.at("assignments"));
    j.at("description").get_to(obj.description);
    j.at("code").get_to(obj.code);
}

void tt::to_json(nlohmann::json &j, const State &obj)
{
    j = {
        {"assignments", obj.assignments},
        {"description", obj.description},
        {"code", obj.code},
    };
}
