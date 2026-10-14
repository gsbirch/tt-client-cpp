#pragma once

#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include <tt/io/Message.h>

#ifndef MESSAGEFACTORY_H
#define MESSAGEFACTORY_H

namespace tt {
    /**
     * A helper class for deserializing {@link tt::Message messages}. This class
     * helps the client {@link tt::Client::receiveAny create an object of the correct Message type} 
     * for incoming JSON.
     * 
     * Custom clients should not need to use this class or any of its member functions, as message
     * receiving is already built into the Client class.
     */
    class MessageFactory {
        public:
            /** Type alias to simplify code */
            using Creator = std::function<std::unique_ptr<tt::Message>(const nlohmann::json&)>;

            /**
             * MessageFactory singleton.
             */
            static MessageFactory& instance()
            {
                static MessageFactory factory;
                return factory;
            }

            /**
             * Register a type with the message factory.
             * This is called when the first {@link tt::Client} is created.
             * 
             * @param type the name of the type
             * @param creator a function to create an object of that type
             */
            void registerType(const std::string& type, Creator creator)
            {
                creators_[type] = std::move(creator);
            }

            /**
             * Looks up the creator of the desired type to deserialize json.
             * Uses the {@link #createFromJson} functions generated for each type.
             * 
             * @param type the name of the type
             * @param j json serialization of the message
             * @return a pointer to the message created
             */
            std::unique_ptr<tt::Message> create(const std::string& type, const nlohmann::json& j) const
            {
                auto it = creators_.find(type);
                if (it == creators_.end()) {
                    return nullptr; // unknown type
                }
                return it->second(j);
            }

        private:
            /** A dictionary of all the creator functions */
            std::unordered_map<std::string, Creator> creators_;
    };

    /**
     * Creates a message of a specified type from JSON.
     * 
     * @tparam T the type of the message serialized
     * @param j JSON of the serialized message
     * @return a pointer to the newly created message
     */
    template <typename T>
    std::unique_ptr<Message> createFromJson(const nlohmann::json& j)
    {
        auto msg = std::make_unique<T>();
        from_json(j, *msg);  
        return msg;
    }

    /**
     * Helper function to register all message types.
     * Runs when the first {@link tt::Client} is created.
     */
    void registerMessageTypes();
}

#endif