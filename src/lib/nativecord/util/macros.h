#pragma once

/*
    helper macros
*/
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

/*
    compiler specific stuff
*/
#ifdef _MSC_VER
    #define NC_NORET __declspec(noreturn)
#elif defined(__GNUC__)
    #define NC_NORET [[noreturn]]
#else
    #define NC_NORET [[noreturn]]
#endif

#ifdef NATIVECORD_LIB_SHARED
    #ifdef _MSC_VER
        #define NC_EXPORT __declspec(dllexport)
    #elif defined(__GNUC__)
        #if __GNUC__ >= 4
            #define NC_EXPORT __attribute__((visibility("default")))
        #else
            #define NC_EXPORT __attribute__((dllexport))
        #endif
    #else
        #define NC_EXPORT
    #endif
#else
    #define NC_EXPORT
#endif