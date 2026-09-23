#include <tt/world/Ending.h>
#include <tt/util/JsonUtil.h>

using namespace tt;

std::string tt::Ending::toString() const
{
    return name;
}

std::ostream &tt::operator<<(std::ostream &os, const Ending &a)
{
    os << a.toString();
    return os;
}

void tt::from_json(const nlohmann::json &j, Ending &obj)
{
    j.at("id").get_to(obj.id);
    j.at("name").get_to(obj.name);
    fromJsonPtr(j, "signature", obj.signature);
    j.at("description").get_to(obj.description);
    j.at("code").get_to(obj.code);
}

void tt::to_json(nlohmann::json &j, const Ending &obj)
{
    j = {
        {"id", obj.id},
        {"name", obj.name},
        {"signature", *obj.signature},
        {"description", obj.description},
        {"code", obj.code}
    };
}
