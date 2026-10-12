#include <tt/world/Entity.h>

using namespace tt;

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

const Entity *tt::Entity::toEntity() const
{
    return this;
}

std::string_view tt::Entity::getType() const
{
    return TYPE;
}

bool tt::Entity::equals(const Value &other) const
{
    const Entity& o = static_cast<const Entity&>(other);
    return id == o.id;
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
