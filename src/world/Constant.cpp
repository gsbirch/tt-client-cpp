#include <tt/world/Constant.h>

using namespace tt;

const Constant Constant::NULL_CONSTANT = Constant(std::monostate());
const Constant Constant::TRUE = Constant(true);
const Constant Constant::FALSE = Constant(false);

std::string tt::Constant::toString() const
{
    if (auto* p = std::get_if<bool>(&value))            return *p ? "true" : "false";
    if (auto* p = std::get_if<std::int64_t>(&value))    return std::to_string(*p);
    if (auto* p = std::get_if<double>(&value))          return std::to_string(*p);
    return "null";
}

bool tt::Constant::toBool() const
{
    return *this == Constant::TRUE;
}

double tt::Constant::toNumber() const
{
    if (auto p = std::get_if<bool>(&value))         return *p ? 1.0 : 0.0;
    if (auto p = std::get_if<std::int64_t>(&value)) return static_cast<double>(*p);
    if (auto p = std::get_if<double>(&value))       return *p;
    return 0.0;  // monostate
}

std::string_view tt::Constant::getType() const
{
    return TYPE;
}

bool tt::Constant::equals(const Value &other) const
{
    const Constant& o = static_cast<const Constant&>(other);
    return value == o.value;
}

void tt::from_json(const nlohmann::json &j, Constant &msg) {
    auto it = j.find("value");
    if (it == j.end() || it->is_null()) msg.value = std::monostate{};
    else if (it->is_boolean())          msg.value = it->get<bool>();
    else if (it->is_number_integer())   msg.value = it->get<std::int64_t>();
    else                                msg.value = it->get<double>();
}

void tt::to_json(nlohmann::json &j, const Constant &msg)
{
    j = {
        {"type", "Constant"},
        {"value", msg.toString()},
    };
}
