#include <tt/world/Turn.h>
#include <tt/util/JsonUtil.h>

using namespace tt;

std::string tt::Turn::toString() const
{
    std::string string = rtos(role) + " " + ttos(type);
		if(action != nullptr)
			string += " " + action->toString();
		return string;
}

std::ostream &tt::operator<<(std::ostream &os, const Turn &a)
{
    os << a.toString();
    return os;
}

void tt::from_json(const nlohmann::json &j, Turn &obj)
{
    obj.role = stor(j.at("role").get<std::string>());
    obj.type = stot(j.at("type").get<std::string>());
    if (obj.type != Turn::Type::PASS) {
        obj.action = registerEntry<Action>(j.at("action"));
    }
    j.at("description").get_to(obj.description);
    j.at("code").get_to(obj.code);
}

void tt::to_json(nlohmann::json &j, const Turn &obj)
{
    j = {
        {"role", rtos(obj.role)},
        {"type", ttos(obj.type)},
        {"description", obj.description},
        {"code", obj.code},
    };
    if (obj.type != Turn::Type::PASS) {
        j["action"] = obj.action;
    }
}

std::string tt::ttos(Turn::Type type)
{
    switch (type)
    {
    case Turn::Type::PROPOSE:
        return "PROPOSE";
    case Turn::Type::SUCCEED:
        return "SUCCEED";
    case Turn::Type::FAIL:
        return "FAIL";
    case Turn::Type::PASS:
        return "PASS";
    default:
        return "Unknown Turn Type";
    }
}

Turn::Type tt::stot(std::string s)
{
    if (s == "PROPOSE")
        return Turn::Type::PROPOSE;
    if (s == "SUCCEED")
        return Turn::Type::SUCCEED;
    if (s == "FAIL")
        return Turn::Type::FAIL;
    if (s == "PASS")
        return Turn::Type::PASS;
    return Turn::Type::PASS;
}
