#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#define NC_USERAGENT "nativecord"
#define NC_HTTP_BUFFSIZE 1024

namespace nativecord
{
    namespace http
    {
        std::unique_ptr<char[]> request(const char* url, const char* method, const char* data = nullptr,
                                        size_t size = 0, std::unordered_map<std::string, std::string>* headers = nullptr,
                                        std::string userAgent = NC_USERAGENT);
    }
}