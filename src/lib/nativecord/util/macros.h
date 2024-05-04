#pragma once

/*
    compiler specific macros
*/
#ifdef _MSC_VER
    #define NC_EXPORT __declspec(dllexport)
    #define NC_NORET __declspec(noreturn)

    #define NC_WARNING_PUSH __pragma(warning(push))
    #define NC_WARNING_POP __pragma(warning(pop))
    #define NC_DISABLE_WARNING(warningNumber) __pragma(warning(disable : warningNumber))
#elif defined(__GNUC__)
    #if __GNUC__ >= 4
        #define NC_EXPORT __attribute__((visibility("default")))
    #else
        #define NC_EXPORT __attribute__((dllexport))
    #endif
    #define NC_NORET [[noreturn]]

    #define NC_WARNING_PUSH _Pragma("GCC diagnostic push")
    #define NC_WARNING_POP _Pragma("GCC diagnostic pop")
    #define NC_DISABLE_WARNING(warningName) _Pragma("GCC diagnostic ignored \"" STR_HELPER(warningName) "\"")
#else
    #define NC_EXPORT
    #define NC_NORET
    #define NC_WARNING_PUSH
    #define NC_WARNING_POP
    #define NC_DISABLE_WARNING(warningNumber)
#endif

/*
    helper macros
*/
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)