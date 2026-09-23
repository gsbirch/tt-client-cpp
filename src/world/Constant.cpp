#include <tt/world/Constant.h>

using namespace tt;

const std::string Constant::type = "Constant";

std::string tt::Constant::toString() const
{
    if (auto* b = std::get_if<bool>(&value))            return *b ? "true" : "false";
    if (auto* i = std::get_if<std::int64_t>(&value))    return std::to_string(*i);
    if (auto* d = std::get_if<double>(&value))          return std::to_string(*d);
    return "null";
}

std::ostream &tt::operator<<(std::ostream &os, const Constant &a)
{
    os << a.toString();
    return os;
}

void tt::from_json(const nlohmann::json &j, Constant &msg) {
    auto it = j.find("value");
    if (it == j.end() || it->is_null()) msg.value = std::monostate{};
    else if (it->is_boolean())          msg.value = it->get<bool>();
    else if (it->is_number_integer())   msg.value = it->get<std::int64_t>();
    else                                msg.value = it->get<float>();
}

void tt::to_json(nlohmann::json &j, const Constant &msg)
{
    j = {
        {"type", "Constant"},
        {"value", msg.toString()},
    };
}
