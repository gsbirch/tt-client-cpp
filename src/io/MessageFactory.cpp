#include <tt/io/Message.h>
#include <tt/io/Join.h>

using MessageFactory =
    std::function<std::unique_ptr<tt::Message>(const tt::json&)>;


static std::unordered_map<std::string, MessageFactory> factories = {
    {
        "Join",
        [](const nlohmann::json& j) -> std::unique_ptr<tt::Message> {
            return std::make_unique<tt::Join>(
                j.get<tt::Join>()
            );
        }
    }
};
