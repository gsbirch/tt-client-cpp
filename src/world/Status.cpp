#include <tt/world/Status.h>
#include <tt/util/JsonUtil.h>

using namespace tt;

std::string tt::Status::toString() const
{
    std::string string = "[" + rtos(role) + " status:";
    string += " " + std::to_string(history.size()) + " turns";
    string += "; " + std::to_string(choices.size()) + " choices";
    return string + "]";
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
    obj.history = registerEntries<Turn>(j.at("history"));
    obj.state = registerEntry<State>(j.at("state"));
    obj.descriptions = registerEntries<Entity>(j.at("descriptions"));
    obj.choices = registerEntries<Turn>(j.at("choices"));
}

void tt::to_json(nlohmann::json &j, const Status &obj)
{
    j = {
        {"role", rtos(obj.role)},
        {"history", obj.history},
        {"state", obj.state},
        {"descriptions", obj.descriptions},
        {"choices", obj.choices},
    };
}
