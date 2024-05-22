#include "embed.h"

#include "nativecord/discord/objects/embed.h"

namespace nativecord
{
    NC_JSON_FUNCS(EmbedFooter, icon_url, proxy_icon_url)
    NC_JSON_FUNCS(EmbedMedia, url, proxy_url, height, width)
    NC_JSON_FUNCS(EmbedProvider, name, url)
    NC_JSON_FUNCS(EmbedAuthor, name, url, icon_url, proxy_icon_url)
    NC_JSON_FUNCS(Embed, title, type, description, url, timestamp, color, footer, image, thumbnail, video, provider,
                  author, fields)

    void to_json(nlohmann::json& nlohmann_json_j, const EmbedField& nlohmann_json_t)
    {
        NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NC_TO_JSON, name, value))
        toJson("inline", nlohmann_json_j, nlohmann_json_t.inline_);
    }
    void from_json(const nlohmann::json& nlohmann_json_j, EmbedField& nlohmann_json_t)
    {
        NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NC_FROM_JSON, name, value))
        fromJson("inline", nlohmann_json_j, nlohmann_json_t.inline_);
    }
}
