#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "nativecord/util/macros.h"

typedef std::string snowflake;

namespace nativecord
{
    class Client;
}

class ObjectBase
{
    public:
        ObjectBase() : _client(nullptr){};
        ObjectBase(nativecord::Client* client) : _client(client){};

        inline nativecord::Client* getClient() { return _client; }

    protected:
        nativecord::Client* _client;
};

/*
    serialization with std::optional
    inspired by:
        https://www.kdab.com/jsonify-with-nlohmann-json/
*/
template <typename> constexpr bool isOptional = false;
template <typename T> constexpr bool isOptional<std::optional<T>> = true;

template <class T, class J> inline void optionalToJson(J& j, const char* name, const std::optional<T>& value)
{
    if (value)
        j[name] = *value;
}

template <class T, class J> inline void toJson(const char* key, J& j, const T& value)
{
    if constexpr (isOptional<T>)
        optionalToJson(j, key, value);
    else
        j[key] = value;
}

template <class T, class J> inline void optionalFromJson(J& j, const char* name, std::optional<T>& value)
{
    const auto it = j.find(name);
    if (it != j.end())
        value = it->template get<T>();
    else
        value = std::nullopt;
}

template <typename T, class J> inline void fromJson(const char* key, const J& j, T& value)
{
    if (!j.contains(key))
        return;

    if (j.at(key).is_null())
        return;

    if constexpr (isOptional<T>)
        optionalFromJson(j, key, value);
    else
        j.at(key).get_to(value);
}

#define NC_TO_JSON(val) toJson(#val, nlohmann_json_j, nlohmann_json_t.val);
#define NC_FROM_JSON(val) fromJson(#val, nlohmann_json_j, nlohmann_json_t.val);

#define NC_JSON_DECLFUNCS(Type, ...)                                                                                   \
    inline void to_json(nlohmann::json& nlohmann_json_j, const Type& nlohmann_json_t)                                  \
    {                                                                                                                  \
        NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NC_TO_JSON, __VA_ARGS__))                                             \
    }                                                                                                                  \
                                                                                                                       \
    inline void from_json(const nlohmann::json& nlohmann_json_j, Type& nlohmann_json_t)                                \
    {                                                                                                                  \
        NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NC_FROM_JSON, __VA_ARGS__));                                          \
    }
