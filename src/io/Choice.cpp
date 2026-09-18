#include <tt/io/Choice.h>

tt::Choice::Choice(int index):
index(index)
{
}

std::string tt::Choice::toString() const
{
    std::string string = "[Choice Message:";
    if (getClient() != nullptr)
        string += " \"" + getClient()->getName() + "\"";
    string += " " + index;
    return string + "]";
}

void tt::Choice::verify() const
{
    // Utilities.requireNonNegative(index, "choice");
}

void tt::to_json(json &j, const Choice &msg)
{
    j = {
        {"type", msg.type()},
        {"index", std::to_string(msg.index)},
    };
}

void tt::from_json(const json &j, Choice &msg)
{
    j.at("index").get_to(msg.index);
}
