#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "nativecord/util/macros.h"

#define NC_USERAGENT "nativecord"
#define NC_HTTP_BUFFSIZE 1024

namespace nativecord
{
    namespace http
    {
        NC_EXPORT std::unique_ptr<char[]> request(const char* url, const char* method, const char* data = nullptr,
                                                  size_t size = 0,
                                                  std::unordered_map<std::string, std::string>* headers = nullptr,
                                                  std::string userAgent = NC_USERAGENT);
    }
}