#pragma once

#include <optional>
#include <string>
#include <vector>

#include "nativecord/internal/serializers/embed.h"
#include "nativecord/internal/objectbase.h"

namespace nativecord
{
    struct EmbedFooter
    {
            std::string text;
            std::optional<std::string> icon_url;
            std::optional<std::string> proxy_icon_url;
    };

    struct EmbedMedia
    {
            std::string url;
            std::optional<std::string> proxy_url;
            std::optional<int> height;
            std::optional<int> width;
    };

    struct EmbedProvider
    {
            std::optional<std::string> name;
            std::optional<std::string> url;
    };

    struct EmbedAuthor
    {
            std::string name;
            std::optional<std::string> url;
            std::optional<std::string> icon_url;
            std::optional<std::string> proxy_icon_url;
    };

    struct EmbedField
    {
            std::string name;
            std::string value;
            std::optional<bool> inline_;
    };

    class Embed
    {
        public:
            NC_EXPORT void addField(std::string name, std::string value, std::optional<bool> isInline = std::nullopt);
            NC_EXPORT void setColor(uint8_t r, uint8_t g, uint8_t b);
            NC_EXPORT void getColor(uint8_t& r, uint8_t& g, uint8_t& b);

            std::optional<std::string> title;
            std::optional<std::string> type;
            std::optional<std::string> description;
            std::optional<std::string> url;
            std::optional<std::string> timestamp;
            std::optional<int> color;
            std::optional<EmbedFooter> footer;
            std::optional<EmbedMedia> image;
            std::optional<EmbedMedia> thumbnail;
            std::optional<EmbedMedia> video;
            std::optional<EmbedProvider> provider;
            std::optional<EmbedAuthor> author;
            std::optional<std::vector<EmbedField>> fields;
    };
}
