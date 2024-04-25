#pragma once
#include "macros.h"

NC_NORET void UTIL_Error(const char* err);

#ifdef _DEBUG
    #define ASSERT(cond, msg)                                                                                          \
        do                                                                                                             \
        {                                                                                                              \
            if (!(cond))                                                                                               \
                UTIL_Error(msg "\nFile: " __FILE__ "\nLine: " STR(__LINE__));                                               \
        } while (false)
#else
    #define ASSERT(cond, msg)                                                                                          \
        do                                                                                                             \
        {                                                                                                              \
            if (!(cond))                                                                                               \
                UTIL_Error(Msg);                                                                                       \
        } while (false)
#endif

#ifdef _DEBUG
    #define DEBUG_ASSERT(Cond, Msg) ASSERT(Cond, Msg)
#else
    #define DEBUG_ASSERT(...)
#endif

#define PANIC(Msg) UTIL_Error(Msg);