#pragma once

#include "nativecord/util/macros.h"

#include <hv/httpdef.h>

#include <string>
#include <unordered_map>
#include <optional>

#define NC_USERAGENT "nativecord"

namespace nativecord
{
    namespace http
    {
        std::pair<int, std::string> req(std::string url, http_method method, std::optional<std::string> body = std::nullopt,
                                         std::unordered_map<std::string, std::string>* headers = nullptr,
                                         std::string userAgent = NC_USERAGENT);
    }
}