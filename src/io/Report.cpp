#include <tt/io/Report.h>

tt::Report::Report(const std::string& item, const std::string& value, const std::string& comment):
item(item), value(value), comment(comment)
{
}

std::string tt::Report::toString() const
{
    return "[Report Message: \"" + item + "\"=\"" + value + "\"; \"" + comment + "\"" + "]";
}

void tt::Report::verify() const
{
    // Utilities.requireNonNull(item, "item
	// Utilities.requireNonNull(value, "value");
}

void tt::from_json(const nlohmann::json &j, Report &msg)
{
    j.at("item").get_to(msg.item);
    j.at("value").get_to(msg.value);
    j.at("comment").get_to(msg.comment);
}

void tt::to_json(nlohmann::json &j, const Report &msg)
{
    j = {
        {"type", msg.type()},
        {"item", msg.item},
        {"value", msg.value},
        {"comment", msg.comment},
    };
}
