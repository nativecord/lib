#pragma once

#include <nlohmann/json.hpp>

#include <stdint.h>
#include <string>

struct Overwrite
{
    public:
        uint64_t id;
        int type;
        std::string allow;
        std::string deny;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Overwrite, id, type, allow, deny);