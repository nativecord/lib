#pragma once

/*
    compiler specific macros
*/
#ifdef NATIVECORD_LIB_SHARED
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
