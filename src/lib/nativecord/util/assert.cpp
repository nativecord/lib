#include "nativecord/util/assert.h"

#include <cstdio>
#include <cstdlib>

NC_NORET void UTIL_Error(const char* err)
{
    printf_s("Assertion hit: %s\n", err);

#ifdef _MSC_VER
    __debugbreak();
#else
    __builtin_trap();
#endif

    exit(0);
}