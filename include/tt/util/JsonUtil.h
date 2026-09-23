#include <variant>
#include <memory>
#include <tt/world/Constant.h>
#include <tt/world/Entity.h>
#include <tt/world/Registry.h>

namespace tt {
    using Value = std::variant<std::shared_ptr<Constant>, const Entity*>;
}

namespace nlohmann {
template <>
struct adl_serializer<tt::Value> {
    static void to_json(json& j, const tt::Value& v) {
        std::visit([&](const auto& x) {
            using T = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<T, std::shared_ptr<tt::Constant>>) {
                if (!x) throw std::invalid_argument("null constant");
                j = *x;
                j["type"] = tt::Constant::type;
            } else {
                if (!x) throw std::invalid_argument("null entity");
                j = {{"type", tt::Entity::type}, {"code", x->code}};
            }
        }, v);
    }

    static void from_json(const json& j, tt::Value& v) {
        const auto& type = j.at("type").get_ref<const std::string&>();
        if (type == tt::Constant::type) {
            v = std::make_shared<tt::Constant>(j.get<tt::Constant>());
        } else if (type == tt::Entity::type) {
            auto code = j.at("code").get<std::string>();
            const tt::Entity* e = tt::Registry::getEntity(code);
            if (!e) throw std::runtime_error("unknown entity code: " + code);
            v = e;
        } else {
            throw std::runtime_error("unknown value type: " + type);
        }
    }
};
}

namespace tt {
    // This code makes it easier to get a unique pointer to a newly deserialized object.
    // This is useful for Signatures, Constants, etc.
    template <typename T>
    void fromJsonPtr(const nlohmann::json& j, const char* key, std::unique_ptr<T>& p) {
        const auto &v = j.at(key);
        p = std::make_unique<T>(v.get<T>());
    }

    template <typename T>
    void fromJsonInRegistryVector(const nlohmann::json& j, const char* key, std::vector<const T*>& p) {
        for (const auto& aj : j.at("consenting")) {
            std::string code = aj.at("code");
            p.push_back(tt::Registry::getEntity(code));
        }
    }

    template <typename T>
    void toJsonInVector(nlohmann::json& j, const char* key, const std::vector<const T*>& p) {
        nlohmann::json vecJson = nlohmann::json::array();
        for (const T* a : p) {
            if (a) vecJson.push_back(*a);
        }
        j[key] = std::move(vecJson);
    }

    // dispatch function to make it easier to register an entry in a generic helper function
    template <typename T>
    const T* registerEntry(const nlohmann::json& j) {
        std::string code = j.at("code").get<std::string>();
        if constexpr (std::is_same<T, Action>) {
            if (const Action* e = Registry::getAction(code))
                return e;
            return Registry::registerAction(std::make_unique<Action>(j.get<Action>()));
        }
        if constexpr (std::is_same_v<T, Assignment>) {
            if (const Assignment* e = Registry::getAssignment(code)) {
                return e;
            }
            return Registry::registerAssignment(std::make_unique<Assignment>(j.get<Assignment>()));
        }
        if constexpr (std::is_same_v<T, Ending>) {
            if (const Ending* e = Registry::getEnding(code)) {
                return e;
            }
            return Registry::registerEnding(std::make_unique<Ending>(j.get<Ending>()));
        }
        if constexpr (std::is_same_v<T, Entity>) {
            if (const Entity* e = Registry::getEntity(code)) {
                return e;
            }
            return Registry::registerEntity(std::make_unique<Entity>(j.get<Entity>()));
        }
        if constexpr (std::is_same_v<T, State>) {
            if (const State* e = Registry::getState(code)) {
                return e;
            }
            return Registry::registerState(std::make_unique<State>(j.get<State>()));
        }
        if constexpr (std::is_same_v<T, Turn>) {
            if (const Turn* e = Registry::getTurn(code)) {
                return e;
            }
            return Registry::registerTurn(std::make_unique<Turn>(j.get<Turn>()));
        }
        if constexpr (std::is_same_v<T, Variable>) {
            int id = j.at("id").get<int>();
            if (const Variable* e = Registry::getVariable(id)) {
                return e;
            }
            return Registry::registerVariable(std::make_unique<Variable>(j.get<Variable>()));
        }
    }

    template <typename T>
    std::vector<const T*> registerEntries(const nlohmann::json& arr) {
        std::vector<const T*> result;
        result.reserve(arr.size());
        for (const auto& j : arr) {
            result.push_back(registerEntry<T>(j));
        }
        return result;
    }
}
