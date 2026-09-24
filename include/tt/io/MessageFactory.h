#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include <tt/io/Message.h>

namespace tt {
    class MessageFactory {
        public:
            using Creator = std::function<std::unique_ptr<tt::Message>(const nlohmann::json&)>;

            static MessageFactory& instance()
            {
                static MessageFactory factory;
                return factory;
            }

            void registerType(const std::string& type, Creator creator)
            {
                creators_[type] = std::move(creator);
            }

            std::unique_ptr<tt::Message> create(const std::string& type, const nlohmann::json& j) const
            {
                auto it = creators_.find(type);
                if (it == creators_.end()) {
                    return nullptr; // unknown type
                }
                return it->second(j);
            }

        private:
            std::unordered_map<std::string, Creator> creators_;
    };

    template <typename T>
    std::unique_ptr<Message> createFromJson(const nlohmann::json& j)
    {
        auto msg = std::make_unique<T>();
        from_json(j, *msg);  
        return msg;
    }

    void registerMessageTypes();
}
