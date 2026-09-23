#include <tt/world/Entity.h>

using namespace tt;

const std::string Entity::type = "Entity";

tt::Entity::Entity(int id, std::string name, std::string description, std::string code):
id(id), name(name), description(description), code(code)
{
}

bool tt::Entity::isPlayer() const
{
    return id == 0;
}

std::string tt::Entity::toString() const
{
    return name;
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
