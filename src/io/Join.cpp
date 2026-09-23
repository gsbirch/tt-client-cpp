#include <tt/io/Join.h>
#include <tt/Role.h>

using namespace tt;

/**
 * Constructs a new join message with an agent's credentials and
 * preferences.
 * 
 * @param name the new agent's name
 * @param password the new agent's password, or null if they are not using
 * a reserved name
 * @param world the world the new agent wants to play in, or null if they
 * have no preference
 * @param role the role the new agent wants to have, or null if they have no
 * preference
 * @param partner the name of the partner this new agent wants to play with,
 * or null if they have no preference
 */
Join::Join(std::string name, std::string password, std::string world, Role role, std::string partner):
name(name), password(password), world(world), role(role), partner(partner) {
}

// tt::Join::Join():
// name(""), password(""), world(""), role(Role::NONE), partner("")
// {
// }

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

void Join::verify() const {
    // Utilities.requireName(name);
    // if(world != null)
    // 	Utilities.requireName(world);
    // if(partner != null)
    // 	Utilities.requireName(partner);
}

/**
 * Returns true if this join request is compatible with another, meaning a
 * session could be created between the agents who sent the messages. This
 * method will not match two requests which both specify no preference for a
 * partner. In other words, at least one of the requests must specify a
 * partner.
 * 
 * @param other the join message of another agent waiting for a session
 * @return true if the join messages are compatible, or false if they are
 * not compatible
 */
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

// std::ostream &tt::operator<<(std::ostream &os, const Join &a)
// {
//     os << a.toString();
//     return os;
// }

void tt::to_json(json &j, const Join &msg)
{
    j = {
        {"type", msg.type()},
        {"name", msg.name},
        {"password", msg.password},
        {"world", msg.world},
        {"role", rtos(msg.role)},
        {"partner", msg.partner},
    };
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


