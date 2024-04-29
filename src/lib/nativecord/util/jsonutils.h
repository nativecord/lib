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

