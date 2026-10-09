#include <tt/world/Action.h>
#include <tt/world/Entity.h>
#include <tt/util/JsonUtil.h>
#include <tt/world/Turn.h>
#include <tt/util/Util.h>

using namespace tt;

const std::unordered_set<const Entity *> &tt::Action::getConsenting()
{
    if (consentingSet.empty()) {
        for (const auto& e : consenting) {
            consentingSet.insert(e.get());
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

const std::string& tt::Action::toString() const
{
    return name;
}

const std::string& tt::Action::getDescription() const
{
    return description;
}

const std::string &tt::Action::getCode() const
{
    return code;
}

const Signature *tt::Action::getSignature() const
{
    return signature.get();
}

bool tt::Action::operator==(const Action &rhs) const
{
    return id == rhs.id && name == rhs.name;
}

std::ostream &tt::operator<<(std::ostream &os, const Action &a)
{
    os << a.toString();
    return os;
}

void tt::from_json(const nlohmann::json &j, Action &obj)
{
    j.at("id").get_to(obj.id);
    requireNonNegative(obj.id, "ID");
    j.at("name").get_to(obj.name);
    requireNonEmpty(obj.name, "name");
    j.at("description").get_to(obj.description);
    j.at("code").get_to(obj.code);
    fromJsonPtr(j, "signature", obj.signature);
    fromJsonPtrVec(j, "consenting", obj.consenting);
}

void tt::to_json(nlohmann::json &j, const Action &obj)
{
    j = {
        {"id", obj.id},
        {"name", obj.name},
        {"signature", *obj.signature},
        {"description", obj.description},
        {"code", obj.code},
    };
    toJsonInVector(j, "consenting", obj.consenting);
}
