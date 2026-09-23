#include <tt/world/Action.h>
#include <tt/world/Registry.h>
#include <tt/world/Entity.h>
#include <tt/util/JsonUtil.h>
#include "Turn.h"

using namespace tt;

tt::Action::Action(int id, std::unique_ptr<Signature> signature, std::vector<const Entity *> consenting, std::string description, std::string code):
id(id), consenting(consenting), description(description), code(code)
{
    this->signature = std::move(signature);
}

const std::unordered_set<const Entity *> &tt::Action::getConsenting()
{
    if (consentingSet.empty()) {
        for (const auto& e : consenting) {
            consentingSet.insert(e);
        }
    }
    return consentingSet;
}

bool tt::Action::consents(Role role)
{
    auto consenting = getConsenting();
    bool gm = consenting.size() == 0;
    bool player = false;
    for(const Entity* character : getConsenting()) {
        if(character->isPlayer())
            player = true;
        else
            gm = true;
    }
    if(role == Role::GAME_MASTER)
        return gm;
    else
        return player;
}

std::string tt::Action::toString() const
{
    return name;
}

std::ostream &tt::operator<<(std::ostream &os, const Action &a)
{
    os << a.toString();
    return os;
}

void tt::from_json(const nlohmann::json &j, Action &obj)
{
    j.at("id").get_to(obj.id);
    j.at("name").get_to(obj.name);
    j.at("description").get_to(obj.description);
    j.at("code").get_to(obj.code);
    fromJsonPtr(j, "signature", obj.signature);
    obj.consenting = registerEntries<Entity>(j.at("consenting"));
}

void tt::to_json(nlohmann::json &j, const Action &obj)
{
    j = {
        {"id", obj.id},
        {"name", obj.name},
        {"signature", *obj.signature},
        {"consenting", obj.consenting},
        {"description", obj.description},
        {"code", obj.code},
    };
}
