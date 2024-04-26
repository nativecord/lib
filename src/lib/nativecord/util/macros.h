#pragma once

/*
    compiler specific macros
*/
#if defined(_WIN32) || defined(__CYGWIN__)
    #ifdef __GNUC__
        #define NC_EXPORT __attribute__((dllexport))
    #else
        #define NC_EXPORT __declspec(dllexport)
    #endif
#elif defined(__GNUC__) && __GNUC__ >= 4
    #define NC_EXPORT __attribute__((visibility("default")))
#else
    #define NC_EXPORT
#endif

#ifdef __GNUC__
    #define NC_NORET [[noreturn]]
#elif defined _WIN32
    #define NC_NORET __declspec(noreturn)
#else
    #define NC_NORET [[noreturn]]
#endif

/*
    helper macros
*/
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

/*
    clang error fixes
*/
// under some circumstances [#a] makes clang detect the header as objective-c, so this macro does it somewhere else
#define _ADD_BRACKETS(a) [a]

/*
    json macros

    TO-DO:
        find a cleaner way to do serialization
*/
#define _NC_SERIALIZE(key, val, obj, js) js##key = obj.##val;
#define _NC_SERIALIZE_OPT(key, val, obj, js, cond)                                                                     \
    if (cond)                                                                                                          \
    _NC_SERIALIZE(key, val, obj, js)

#define NC_SERIALIZE(key) js _ADD_BRACKETS(#key) = obj.##key;
#define NC_SERIALIZE_OPT_STR(key) _NC_SERIALIZE_OPT(_ADD_BRACKETS(STR(key)), key, obj, js, !(obj.##key.empty()))
#define NC_SERIALIZE_OPT_NUM(key) _NC_SERIALIZE_OPT(_ADD_BRACKETS(STR(key)), key, obj, js, (obj.##key != -1))
#define NC_SERIALIZE_OPT_UNSIGNED(key) _NC_SERIALIZE_OPT(_ADD_BRACKETS(STR(key)), key, obj, js, (obj.##key != 0))
#define NC_SERIALIZE_OPT(key) _NC_SERIALIZE_OPT(_ADD_BRACKETS(STR(key)), key, obj, js, (obj.##key))

#define NC_SERIALIZE_CLEANUP(js)                                                                                       \
    for (auto it = js.begin(); it != js.end();)                                                                        \
    {                                                                                                                  \
        if (it.value().is_string())                                                                                    \
        {                                                                                                              \
            if (it.value().get<std::string>().empty())                                                                 \
                it = js.erase(it);                                                                                     \
            continue;                                                                                                  \
        }                                                                                                              \
        if (it.value().is_object())                                                                                    \
        {                                                                                                              \
            if (it.value().empty())                                                                                    \
                it = js.erase(it);                                                                                     \
            continue;                                                                                                  \
        }                                                                                                              \
        if (it.value().is_number_integer())                                                                            \
        {                                                                                                              \
            if (it.value().get<int>() < 0)                                                                             \
                it = js.erase(it);                                                                                     \
            continue;                                                                                                  \
        }                                                                                                              \
        ++it;                                                                                                          \
    }

#define _NC_DESERIALIZE(key, val, obj, js)                                                                             \
    if (js.contains(key) && !js _ADD_BRACKETS(key).is_null())                                                    \
        js _ADD_BRACKETS(key).get_to(obj.##val);

#define NC_DESERIALIZE(key)                                                                                            \
    if (js.contains(#key) && !js _ADD_BRACKETS(STR(key)).is_null())                                                    \
        js _ADD_BRACKETS(#key).get_to(obj.##key);
#define NC_DESERIALIZE_UINT64(key)                                                                                     \
    if (js.contains(#key) && !js _ADD_BRACKETS(#key).is_null())                                                        \
        obj.##key = std::stoull(js[#key].get<std::string>());
