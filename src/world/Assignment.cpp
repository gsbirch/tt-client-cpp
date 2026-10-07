#include <tt/world/Assignment.h>
#include <tt/world/Signature.h>
#include <tt/util/JsonUtil.h>
#include <tt/util/Util.h>

using namespace tt;

std::string tt::Assignment::toString() const
{
    return variable->toString() + " = " + vtos(value);
}

const std::string &tt::Assignment::getDescription() const
{
    return description;
}

const std::string &tt::Assignment::getCode() const
{
    return code;
}

const Variable *tt::Assignment::getVariable() const
{
    return variable.get();
}

bool tt::Assignment::operator==(const Assignment &rhs) const
{
    return *variable == *(rhs.variable) ;
}

std::ostream &tt::operator<<(std::ostream &os, const Assignment &a)
{
    os << a.toString();
    return os;
}

void tt::from_json(const nlohmann::json &j, Assignment &obj)
{
    fromJsonPtr(j, "variable", obj.variable);
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
