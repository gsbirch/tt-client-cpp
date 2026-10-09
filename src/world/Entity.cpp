#include <tt/world/Entity.h>

using namespace tt;

const std::string Entity::type = "Entity";

bool tt::Entity::isPlayer() const
{
    return id == 0;
}

const std::string &tt::Entity::getDescription() const
{
    return description;
}

const std::string &tt::Entity::getCode() const
{
    return code;
}

std::string tt::Entity::toString() const
{
    return name;
}

bool tt::Entity::operator==(const Entity &rhs) const
{
    return id == rhs.id;
}

std::ostream &tt::operator<<(std::ostream &os, const Entity &a)
{
    os << a.toString();
    return os;
}

void tt::from_json(const nlohmann::json &j, Entity &obj)
{
    j.at("id").get_to(obj.id);
    j.at("name").get_to(obj.name);
    j.at("description").get_to(obj.description);
    j.at("code").get_to(obj.code);
}

void tt::to_json(nlohmann::json &j, const Entity &obj)
{
    j = {
        {"type", "Entity"},
        {"id", obj.id},
        {"name", obj.name},
        {"description", obj.description},
        {"code", obj.code},
    };
}
