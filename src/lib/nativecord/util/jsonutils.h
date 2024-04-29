// clang-format off
#pragma once

#include <nlohmann/json.hpp>
#include "macros.h"

#define _NC_SERIALIZE(key, val, obj, js) js##key = obj.##val;
#define _NC_SERIALIZE_OPT(key, val, obj, js, cond)                                                                     \
    if (cond)                                                                                                          \
    _NC_SERIALIZE(key, val, obj, js)

#define NC_SERIALIZE(key) js[#key] = obj.##key;

#define NC_SERIALIZE_OPT(key) _NC_SERIALIZE_OPT(_ADD_BRACKETS(STR(key)), key, obj, js, (obj.##key))
#define NC_SERIALIZE_OPT_STR(key) _NC_SERIALIZE_OPT(_ADD_BRACKETS(STR(key)), key, obj, js, !(obj.##key.empty()))
#define NC_SERIALIZE_OPT_NUM(key) _NC_SERIALIZE_OPT(_ADD_BRACKETS(STR(key)), key, obj, js, (obj.##key != -1))
#define NC_SERIALIZE_OPT_UNSIGNED(key) _NC_SERIALIZE_OPT(_ADD_BRACKETS(STR(key)), key, obj, js, (obj.##key != 0))

#define NC_SERIALIZE_CLEANUP(js)                                                                                       \
    for (auto it = js.begin(); it != js.end();)                                                                        \
    {                                                                                                                  \
        if (it.value().is_string() && it.value().get<std::string>().empty())                                           \
        {                                                                                                              \
            it = js.erase(it);                                                                                         \
            continue;                                                                                                  \
        }                                                                                                              \
        if (it.value().is_object() && it.value().empty())                                                              \
        {                                                                                                              \
            it = js.erase(it);                                                                                         \
            continue;                                                                                                  \
        }                                                                                                              \
        if (it.value().is_number_integer() && it.value().get<int>() < 0)                                               \
        {                                                                                                              \
            it = js.erase(it);                                                                                         \
            continue;                                                                                                  \
        }                                                                                                              \
        ++it;                                                                                                          \
    }

#define _NC_DESERIALIZE(key, val, obj, js)                                                                             \
    if (js.contains(key) && !js _ADD_BRACKETS(key).is_null())                                                          \
        js _ADD_BRACKETS(key).get_to(obj.##val);

#define NC_DESERIALIZE(key)                                                                                            \
    if (js.contains(#key) && !js _ADD_BRACKETS(STR(key)).is_null())                                                    \
        js _ADD_BRACKETS(#key).get_to(obj.##key);
#define NC_DESERIALIZE_UINT64(key)                                                                                     \
    if (js.contains(#key) && !js _ADD_BRACKETS(#key).is_null())                                                        \
        obj.##key = std::stoull(js[#key].get<std::string>());

// clang-format on

/*
    std::contional compatibility
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
        value = it->get<T>();
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

#define NC_JSON_TO_WITH_OPTIONAL(val) toJson(#val, nlohmann_json_j, nlohmann_json_t.val);
#define NC_JSON_FROM_WITH_OPTIONAL(val) fromJson(#val, nlohmann_json_j, nlohmann_json_t.val);

#define NC_JSON_DECLFUNCS(Type, ...)                                                                                   \
    NC_EXPORT inline void to_json(nlohmann::json& nlohmann_json_j, const Type& nlohmann_json_t){                       \
        NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NC_JSON_TO_WITH_OPTIONAL, __VA_ARGS__))}                              \
                                                                                                                       \
    NC_EXPORT inline void from_json(const nlohmann::json& nlohmann_json_j, Type& nlohmann_json_t)                      \
    {                                                                                                                  \
        NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NC_JSON_FROM_WITH_OPTIONAL, __VA_ARGS__));                            \
    }
