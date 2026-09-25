#include <tt/io/Start.h>
#include <tt/util/JsonUtil.h>

using namespace tt;

std::string tt::Start::toString() const
{
    return "[Start Message: world=\"" + world->name + "\"; role=\"" + rtos(role) + "\"]";
}

void tt::Start::verify() const
{

}

void tt::from_json(const nlohmann::json &j, Start &msg)
{
    msg.role = stor(j.at("role").get<std::string>());
    fromJsonPtr(j, "world", msg.world);
}

void tt::to_json(nlohmann::json &j, const Start &msg)
{
    j = {
        {"type", msg.type()},
        {"role", rtos(msg.role)},
        {"world", *msg.world},
    };
}
