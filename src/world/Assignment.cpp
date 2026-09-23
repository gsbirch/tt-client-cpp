#include <tt/world/Assignment.h>
#include <tt/world/Signature.h>
#include <tt/util/JsonUtil.h>
#include <tt/util/Util.h>

using namespace tt;

std::string tt::Assignment::toString() const
{
    return variable->toString() + " = " + vtos(value);
}

std::ostream &tt::operator<<(std::ostream &os, const Assignment &a)
{
    os << a.toString();
    return os;
}

void tt::from_json(const nlohmann::json &j, Assignment &obj)
{
    int id = j.at("variable").at("id").get<int>();
    obj.variable = Registry::getVariable(id);
    j.at("value").get_to(obj.value);
    j.at("visible").get_to(obj.visible);
    j.at("description").get_to(obj.description);
    j.at("code").get_to(obj.code);
}

void tt::to_json(nlohmann::json &j, const Assignment &obj)
{
    j = {
        {"variable", *obj.variable},
        {"visible", obj.visible},
        {"value", obj.value},
        {"description", obj.description},
        {"code", obj.code},
    };
    
}
