#include <tt/world/Value.h>
#include <tt/world/Entity.h>
#include <limits>

using namespace tt;

bool tt::Value::toBool() const
{
    return false;
}

double tt::Value::toNumber() const
{
    return std::numeric_limits<double>::quiet_NaN();
}

const Entity* tt::Value::toEntity() const
{
    return nullptr;
}

std::string tt::Value::toString() const
{
    return "No Value.";
}

std::string_view tt::Value::getType() const
{
    return "No Value.";
}

bool tt::Value::operator==(const Value &other) const
{
    if (typeid(*this) != typeid(other)) return false;
    return equals(other);
}

std::ostream &tt::operator<<(std::ostream &os, const Value &obj)
{
    os << obj.toString();
    return os;
}
