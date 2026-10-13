#pragma once

#include <variant>
#include <memory>
#include <vector>
#include <nlohmann/json.hpp>
#include <tt/world/Value.h>
#include <tt/world/Constant.h>
#include <tt/world/Entity.h>

namespace tt {
    // Value is a simple joint type between Constant, Entity, and null
    // This code allows Value to be used like any other type without interference
    // And allows for equality.,
    /* struct Value : std::variant<std::monostate,
                                std::unique_ptr<Constant>,
                                std::unique_ptr<Entity>> {
        using variant::variant;

        friend bool operator==(const Value& a, const Value& b) {
            return std::visit([](const auto& x, const auto& y) {
                using X = std::decay_t<decltype(x)>;
                using Y = std::decay_t<decltype(y)>;
                if constexpr (!std::is_same_v<X, Y>)
                    return false;
                else if constexpr (std::is_same_v<X, std::monostate>)
                    return true;
                else
                    return (x && y) ? *x == *y : (!x && !y);
            }, static_cast<const variant&>(a), static_cast<const variant&>(b));
        }
    }; */
};


namespace nlohmann {
template <>
struct adl_serializer<std::unique_ptr<tt::Value>> {

    static void to_json(json& j, const std::unique_ptr<tt::Value>& v) {
        if (!v) { j = nullptr; return; }

        if (auto* e = dynamic_cast<const tt::Entity*>(v.get())) {
            j = *e;                                   // uses Entity's to_json
        } else if (auto* c = dynamic_cast<const tt::Constant*>(v.get())) {
            j = *c;
        } else {
            throw std::runtime_error("Cannot serialize generic Value");
        }
        j["type"] = std::string(v->getType());           // tag added on top
    }

    static void from_json(const json& j, std::unique_ptr<tt::Value>& v) {
        if (j.is_null()) { v.reset(); return; }

        const auto type = j.at("type").get<std::string>();
        if (type == tt::Entity::TYPE) {
            auto e = std::make_unique<tt::Entity>();
            j.get_to(*e);
            v = std::move(e);
        } else if (type == tt::Constant::TYPE) {
            auto c = std::make_unique<tt::Constant>();
            j.get_to(*c);
            v = std::move(c);
        } else {
            throw std::runtime_error("Unknown Value type: " + type);
        }
    }
};
}

namespace tt {
    // Value-based overload: builds a unique_ptr from a json value
    template <typename T>
    void fromJsonPtr(const nlohmann::json& v, std::unique_ptr<T>& p) {
        p = std::make_unique<T>(v.get<T>());
    }
    
    // This code makes it easier to get a unique pointer to a newly deserialized object.
    // This is useful for Signatures, Constants, etc.
    template <typename T>
    void fromJsonPtr(const nlohmann::json& j, const char* key, std::unique_ptr<T>& p) {
        fromJsonPtr(j.at(key), p);
    }

    // Populates a vector of unique_ptrs from a JSON array stored under `key`
    template <typename T>
    void fromJsonPtrVec(const nlohmann::json& j, const char* key,
                        std::vector<std::unique_ptr<T>>& out) {
        const auto& arr = j.at(key);
        if (!arr.is_array()) {
            throw nlohmann::json::type_error::create(
                302, std::string("'") + key + "' is not an array", &arr);
        }

        out.clear();
        out.reserve(arr.size());
        for (const auto& elem : arr) {
            std::unique_ptr<T> p;
            fromJsonPtr(elem, p);
            out.push_back(std::move(p));
        }
    }
    template <typename T>
    void toJsonInVector(nlohmann::json& j, const char* key, const std::vector<std::unique_ptr<T>>& p) {
        nlohmann::json vecJson = nlohmann::json::array();
        for (const auto& a : p) {
            if (a) vecJson.push_back(*a);
        }
        j[key] = std::move(vecJson);
    }
}