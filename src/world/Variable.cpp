#include <tt/world/Variable.h>
#include <tt/util/JsonUtil.h>

tt::Variable::Variable():
id(0), signature(nullptr)
{
}

std::string tt::Variable::toString() const
{
    return name;
}

std::ostream &tt::operator<<(std::ostream &os, const Variable &a)
{
    os << a.toString();
    return os;
}

void tt::from_json(const nlohmann::json &j, Variable &obj)
{
    j.at("id").get_to(obj.id);
    j.at("name").get_to(obj.name);
    fromJsonPtr(j, "signature", obj.signature);
    j.at("encoding").get_to(obj.encoding);
    j.at("description").get_to(obj.description);
}

void tt::to_json(nlohmann::json &j, const Variable &obj)
{
    j = {
        {"type", "Variable"},
        {"id", obj.id},
        {"name", obj.name},
        {"signature", *obj.signature},
        {"encoding", obj.encoding},
        {"description", obj.description},
    };
}
