#include <tt/world/State.h>
#include <tt/world/Assignment.h>
#include <tt/util/JsonUtil.h>

using namespace tt;

std::string tt::State::toString() const
{
    std::string string = "[";
    for (int i = 0; i < assignments.size(); i++) {
        const Assignment * a = assignments[i].get();
        string += a->toString();
    }
    return string;
}

const std::string &tt::State::getDescription() const
{
    return description;
}

const std::string &tt::State::getCode() const
{
    return code;
}

const std::vector<const Assignment *> tt::State::getAssignments() const
{
    if (assignmentsPtrs.size() != assignments.size()) {
        assignmentsPtrs.clear();
        assignmentsPtrs.reserve(assignments.size());
        for (const auto& p : assignments) {
            assignmentsPtrs.push_back(p.get());
        }
    }
    return assignmentsPtrs;
}

const Value *tt::State::get(const Variable* variable) const
{
    for (const auto& p : assignments) {
        if (*p->getVariable() == *variable) {
            if (p->value == std::monostate())
                // the value is "null"
                return nullptr;
            return &(p->value);;
        }
    }
    // We couldn't find the variable
    return nullptr;
}

bool tt::State::operator==(const State &rhs) const
{
    // Compare both the assignment arrays elementwise
    return std::equal(assignments.begin(), assignments.end(), rhs.assignments.begin(), rhs.assignments.end(),
                    [](const auto& x, const auto& y) {return *x == *y});
}

std::ostream &tt::operator<<(std::ostream &os, const State &a)
{
    os << a.toString();
    return os;
}

void tt::from_json(const nlohmann::json &j, State &obj)
{
    fromJsonPtrVec(j, "assignments", obj.assignments);
    j.at("description").get_to(obj.description);
    j.at("code").get_to(obj.code);
}

void tt::to_json(nlohmann::json &j, const State &obj)
{
    j = {
        {"description", obj.description},
        {"code", obj.code},
    };
    toJsonInVector(j, "assignments", obj.assignments);
}
