#pragma once

/*
    helper macros
*/
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

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

#ifdef _MSC_VER
    #define NC_WARNING_PUSH __pragma(warning(push))
    #define NC_WARNING_POP __pragma(warning(pop))
    #define NC_DISABLE_WARNING(warningNumber) __pragma(warning(disable : warningNumber))
#elif defined(__GNUC__)
    #define NC_WARNING_PUSH _Pragma("GCC diagnostic push")
    #define NC_WARNING_POP _Pragma("GCC diagnostic pop")
    #define NC_DISABLE_WARNING(warningName)                                                                            \
        _Pragma("GCC diagnostic ignored \"" STR_HELPER(warningName) "\"")
#else
    #define NC_WARNING_PUSH
    #define NC_WARNING_POP
    #define NC_DISABLE_WARNING(warningNumber)
#endif

/*
    clang error fixes
*/
// under some circumstances [#a] makes clang detect stuff as objective-c
#define _ADD_BRACKETS(a) [a]
