#pragma once

#include "nativecord/util/jsonutils.h"
#include <stdint.h>
#include <string>

struct Overwrite
{
        uint64_t id;
        int type;
        std::string allow;
        std::string deny;
};
NC_JSON_DECLFUNCS(Overwrite, id, type, allow, deny);