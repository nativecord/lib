#pragma once

#include "nativecord/util/jsonutils.h"

#include <string>
#include <vector>
#include <optional>

struct EmbedFooter
{
        std::string text;
        std::string icon_url;
        std::string proxy_icon_url;
};
NC_JSON_DECLFUNCS(EmbedFooter, text, icon_url, proxy_icon_url);

struct EmbedMedia
{
        std::string url;
        std::optional<std::string> proxy_url;
        std::optional<int> height = 1;
        std::optional<int> width = -1;
};
NC_JSON_DECLFUNCS(EmbedMedia, url, proxy_url, height, width);

struct EmbedProvider
{
        std::optional<std::string> name;
        std::optional<std::string> url;
};
NC_JSON_DECLFUNCS(EmbedProvider, name, url);

struct EmbedAuthor
{
        std::string name;
        std::optional<std::string> url;
        std::optional<std::string> icon_url;
        std::optional<std::string> proxy_icon_url;
};
NC_JSON_DECLFUNCS(EmbedAuthor, name, url, icon_url, proxy_icon_url);

struct EmbedField
{
        std::string name;
        std::string value;
        bool isInline; // optional
};

NC_EXPORT inline void to_json(nlohmann::json& js, const EmbedField& obj)
{
    NC_SERIALIZE(name);
    NC_SERIALIZE(value);
    _NC_SERIALIZE_OPT(["inline"], isInline, obj, js, obj.isInline);
}

NC_EXPORT inline void from_json(const nlohmann::json& js, EmbedField& obj)
{
    NC_DESERIALIZE(name);
    NC_DESERIALIZE(value);
    _NC_DESERIALIZE("inline", isInline, obj, js);
}

struct Embed
{
        std::optional<std::string> title;
        std::optional<std::string> type;
        std::optional<std::string> description;
        std::optional<std::string> url;
        std::optional<std::string> timestamp;
        std::optional<int> color = -1;
        std::optional<EmbedFooter> footer;
        std::optional<EmbedMedia> image;
        std::optional<EmbedMedia> thumbnail;
        std::optional<EmbedMedia> video;
        std::optional<EmbedProvider> provider;
        std::optional<EmbedAuthor> author;
        std::optional<std::vector<EmbedField>> fields;
};
NC_JSON_DECLFUNCS(Embed, title, type, description, url, timestamp, color, footer, image, thumbnail, video, provider,
                  author, fields);