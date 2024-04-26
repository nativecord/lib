#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "nativecord/util/macros.h"

struct EmbedFooter
{
    public:
        std::string text;
        std::string icon_url;
        std::string proxy_icon_url;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(EmbedFooter, text, icon_url, proxy_icon_url);

struct EmbedMedia
{
    public:
        std::string url;
        std::string proxy_url; // optional
        int height = 1;        // optional
        int width = -1;        // optional
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(EmbedMedia, url, proxy_url, height, width);

struct EmbedProvider
{
    public:
        std::string name; // optional
        std::string url;  // optional
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(EmbedProvider, name, url);

struct EmbedAuthor
{
    public:
        std::string name;
        std::string url;            // optional
        std::string icon_url;       // optional
        std::string proxy_icon_url; // optional
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(EmbedAuthor, name, url, icon_url, proxy_icon_url);

struct EmbedField
{
    public:
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
    public:
        std::string title;              // optional
        std::string type;               // optional
        std::string description;        // optional
        std::string url;                // optional
        std::string timestamp;          // optional
        int color = -1;                 // optional
        EmbedFooter footer;             // optional
        EmbedMedia image;               // optional
        EmbedMedia thumbnail;           // optional
        EmbedMedia video;               // optional
        EmbedProvider provider;         // optional
        EmbedAuthor author;             // optional
        std::vector<EmbedField> fields; // optional
};

NC_EXPORT inline void to_json(nlohmann::json& js, const Embed& obj)
{
    NC_SERIALIZE(title);
    NC_SERIALIZE(type);
    NC_SERIALIZE(description);
    NC_SERIALIZE(url);
    NC_SERIALIZE(timestamp);
    NC_SERIALIZE(color);
    NC_SERIALIZE(footer);
    NC_SERIALIZE(image);
    NC_SERIALIZE(thumbnail);
    NC_SERIALIZE(video);
    NC_SERIALIZE(provider);
    NC_SERIALIZE(author);
    NC_SERIALIZE(fields);

    NC_SERIALIZE_CLEANUP(js);
}

NC_EXPORT inline void from_json(const nlohmann::json& js, Embed& obj)
{
    NC_DESERIALIZE(title)
    NC_DESERIALIZE(type);
    NC_DESERIALIZE(description);
    NC_DESERIALIZE(url);
    NC_DESERIALIZE(timestamp);
    NC_DESERIALIZE(color);
    NC_DESERIALIZE(footer);
    NC_DESERIALIZE(image);
    NC_DESERIALIZE(thumbnail);
    NC_DESERIALIZE(video);
    NC_DESERIALIZE(provider);
    NC_DESERIALIZE(author);
    NC_DESERIALIZE(fields);
}