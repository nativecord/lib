#pragma once

#include "nativecord/util/macros.h"

#include <string>

#include <nlohmann/json.hpp>

struct ActivityTimestamps
{
    public:
        int start = -1;
        int end = -1;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ActivityTimestamps, start, end);

struct ActivityEmoji
{
    public:
        std::string name = "";
        uint64_t id = 0;       // optional
        bool animated = false; // optional
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ActivityEmoji, name, id, animated);

struct ActivityParty
{
    public:
        std::string id;      // optional
        int size[2]{-1, -1}; // optional
};
inline void to_json(nlohmann::json& js, const ActivityParty& obj)
{
    NC_SERIALIZE_OPT_STR(id)
    _NC_SERIALIZE_OPT(["size"][0], size[0], obj, js, obj.size[0] > 0);
    _NC_SERIALIZE_OPT(["size"][1], size[1], obj, js, obj.size[1] > 0);
}

inline void from_json(const nlohmann::json& js, ActivityParty& obj)
{
    NC_DESERIALIZE(id);
    NC_DESERIALIZE(size);
}

struct ActivityAssets
{
    public:
        std::string large_image; // optional
        std::string large_text;  // optional
        std::string small_image; // optional
        std::string small_text;  // optional
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ActivityAssets, large_image, large_text, small_image, small_text);

struct ActivitySecrets
{
    public:
        std::string join;     // optional
        std::string spectate; // optional
        std::string match;    // optional
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ActivitySecrets, join, spectate, match);

struct ActivityButton
{
    public:
        std::string label;
        std::string url;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ActivityButton, label, url);

struct Activity
{
    public:
        std::string name;
        int type = -1;
        std::string url; // optional

        int created_at = -1;
        ActivityTimestamps timestamps; // optional
        uint64_t application_id = 0;   // optional
        std::string details;           // optional
        std::string state;             // optional

        ActivityEmoji emoji;     // optional
        ActivityParty party;     // optional
        ActivityAssets assets;   // optional
        ActivitySecrets secrets; // optional

        bool instance = false; // optional
        int64_t flags = -1;    // optional

        ActivityButton buttons[2];
};
//NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Activity, name, type, url, created_at, timestamps, application_id, details, state, emoji, party, assets, secrets, instance, flags)

 NC_EXPORT inline void to_json(nlohmann::json& js, const Activity& obj)
{
    NC_SERIALIZE(name);
    NC_SERIALIZE(type);
    NC_SERIALIZE(url);

    NC_SERIALIZE(created_at);
    NC_SERIALIZE(timestamps);

    NC_SERIALIZE_OPT_UNSIGNED(application_id);
    NC_SERIALIZE(details);
    NC_SERIALIZE(state);

    NC_SERIALIZE(emoji);
    NC_SERIALIZE(party);
    NC_SERIALIZE(assets);
    NC_SERIALIZE(secrets);

    NC_SERIALIZE(instance);
    NC_SERIALIZE(flags);

    NC_SERIALIZE(buttons);

    NC_SERIALIZE_CLEANUP(js);
}

 NC_EXPORT inline void from_json(const nlohmann::json& js, Activity& obj)
{
    NC_DESERIALIZE(name);
    NC_DESERIALIZE(type);
    NC_DESERIALIZE(url);

    NC_DESERIALIZE(created_at);
    NC_DESERIALIZE(timestamps);

    NC_DESERIALIZE(application_id);
    NC_DESERIALIZE(details);
    NC_DESERIALIZE(state);

    NC_DESERIALIZE(emoji);
    NC_DESERIALIZE(party);
    NC_DESERIALIZE(assets);
    NC_DESERIALIZE(secrets);

    NC_DESERIALIZE(instance);
    NC_DESERIALIZE(flags);

    NC_DESERIALIZE(buttons);
 }
