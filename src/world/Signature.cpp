#include <tt/world/Signature.h>
#include <tt/util/JsonUtil.h>
#include <tt/util/Util.h>

using namespace tt;

bool tt::Signature::operator==(const Signature &s) const
{
    bool eq = std::equal(
        arguments.begin(), arguments.end(),
        s.arguments.begin(), s.arguments.end(),
        [](const std::unique_ptr<tt::Value>& a, const std::unique_ptr<tt::Value>& b) {
            return *a == *b;
        }
    );
    return name == s.name && eq;
}

const std::vector<const Value *>& tt::Signature::getArguments() const
{
    if (argumentsPtrs.size() != arguments.size()) {
        argumentsPtrs.clear();
        argumentsPtrs.reserve(arguments.size());
        for (const auto& p : arguments) {
            argumentsPtrs.push_back(p.get());
        }
    }
    return argumentsPtrs;
}

std::string tt::Signature::toString() const
{
    std::string string = name + "(";
    for(int i = 0; i < arguments.size(); i++)
        string += (i == 0 ? "" : ", ") + arguments[i]->toString();
    return string + ")";
}

std::ostream &tt::operator<<(std::ostream &os, const Signature &a)
{
    os << a.toString();
    return os;
}

void tt::from_json(const nlohmann::json &j, Signature &obj)
{
    j.at("name").get_to(obj.name);
    j.at("arguments").get_to(obj.arguments);
}

void tt::to_json(nlohmann::json &j, const Signature &obj)
{
    j = {
        {"name", obj.name},
        {"arguments", obj.arguments},
    };
}