#include <tt/io/Stop.h>
#include <tt/util/JsonUtil.h>

tt::Stop::Stop(const Ending *ending):
ending(ending), role(Role::NONE)
{
    if(ending != nullptr)
        this->message = "The story has ended.";
    else
        this->message = "";
}

tt::Stop::Stop(Role role):
ending(nullptr), role(Role::NONE)
{
    if(role == Role::GAME_MASTER)
        this->message = "The game master ended the session early.";
    else if(role == Role::PLAYER)
        this->message = "The player ended the session early.";
    else
        this->message = "";
}

tt::Stop::Stop(const std::string& message):
ending(nullptr), role(Role::NONE), message(message)
{
}

tt::Stop::Stop():
Stop("")
{
}

std::string tt::Stop::toString() const
{
    return "[Stop Message: \"" + message + "\"]";
}

void tt::Stop::verify() const
{
}

void tt::from_json(const nlohmann::json &j, Stop &msg)
{
    j.at("message").get_to(msg.message);
    if (j.contains("ending")) {
        msg.ending = registerEntry<Ending>(j.at("ending"));
    }
    else {
        msg.role = stor(j.at("role").get<std::string>());
    }
}

void tt::to_json(nlohmann::json &j, const Stop &msg)
{
    j = {
        {"type", msg.type()},
        {"message", msg.message},
    };
    if (msg.ending != nullptr) {
        j["ending"] = *msg.ending;
    }
    else {
        j["role"] = rtos(msg.role);
    }
}
