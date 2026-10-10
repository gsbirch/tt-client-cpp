#include <tt/io/Update.h>
#include <tt/util/JsonUtil.h>

using namespace tt;

std::string tt::Update::toString() const
{
    return "[Update Message: " + std::to_string(status->getChoices().size()) + " choices]";
}

const Status *tt::Update::getStatus() const
{
    return status.get();
}

void tt::from_json(const nlohmann::json &j, Update &msg)
{
    fromJsonPtr(j, "status", msg.status);
}

void tt::to_json(nlohmann::json &j, const Update &msg)
{
    j = {
        {"type", msg.type()},
        {"status", *msg.status},
    };
}
