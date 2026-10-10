#include <tt/io/Join.h>
#include <tt/Role.h>

using namespace tt;

Join::Join(std::string name, std::string password, std::string world, Role role, std::string partner):
name(name), password(password), world(world), role(role), partner(partner) {
}

std::string Join::toString() const {
    std::string string = "[Join Message: name=\"" + name + "\"";
    if(password != "")
        string += "; password";
    if(world != "")
        string += "; world=\"" + world + "\"";
    if(role != Role::NONE)
        string += "; role=\"" + rtos(role) + "\"";
    if(partner != "")
        string += "; partner=\"" + partner + "\"";
    return string + "]";
}

bool Join::matches(Join other) {
    Role p = getPartner(other.role);

    if(!matches(&world, &other.world))
        return false;
    else if(!matches(&role, other.role == Role::NONE ? nullptr : &p))
        return false;
    else if(!matches(&name, &other.partner) || !matches(&partner, &other.name) || (partner == "" && other.partner == ""))
        return false;
    else
        return true;
}

void tt::to_json(json &j, const Join &msg)
{
    j = {
        {"type", msg.type()},
        {"name", msg.name},
        {"password", msg.password},
    };
    if (msg.world != "") j["world"] = msg.world;
    if (msg.role != Role::NONE) j["role"] = rtos(msg.role);
    if (msg.partner != "") j["partner"] = msg.partner;
}

void tt::from_json(const json &j, Join &msg)
{
    j.at("name").get_to(msg.name);
    j.at("password").get_to(msg.password);
    j.at("world").get_to(msg.world);
    std::string rs = "";
    j.at("role").get_to(rs);
    msg.role = stor(rs);
    j.at("partner").get_to(msg.partner);
}