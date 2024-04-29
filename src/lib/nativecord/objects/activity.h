#pragma once

#include "nativecord/util/jsonutils.h"

#include <optional>
#include <string>

struct ActivityTimestamps
{
        int start = -1;
        int end = -1;
};
NC_JSON_DECLFUNCS(ActivityTimestamps, start, end);

struct ActivityEmoji
{
        std::string name = "";
        std::optional<uint64_t> id = 0;
        std::optional<bool> animated = false;
};
NC_JSON_DECLFUNCS(ActivityEmoji, name, id, animated);

struct ActivityParty
{
        std::optional<std::string> id;
        std::optional<int> size;
};
NC_JSON_DECLFUNCS(ActivityParty, id, size);

struct ActivityAssets
{
        std::optional<std::string> large_image;
        std::optional<std::string> large_text;
        std::optional<std::string> small_image;
        std::optional<std::string> small_text;
};
NC_JSON_DECLFUNCS(ActivityAssets, large_image, large_text, small_image, small_text);

struct ActivitySecrets
{
        std::optional<std::string> join;
        std::optional<std::string> spectate;
        std::optional<std::string> match;
};
NC_JSON_DECLFUNCS(ActivitySecrets, join, spectate, match);

struct ActivityButton
{
        std::string label;
        std::string url;
};
NC_JSON_DECLFUNCS(ActivityButton, label, url);

enum activityType
{
    ACTIVITY_GAME = 0,
    ACTIVITY_STREAMING,
    ACTIVITY_LISTENING,
    ACTIVITY_WATCHING,
    ACTIVITY_CUSTOM,
    ACTIVITY_COMPETING
};

struct Activity
{
        std::string name;
        activityType type;
        std::optional<std::string> url;

        int created_at = -1;
        std::optional<ActivityTimestamps> timestamps;
        std::optional<uint64_t> application_id;
        std::optional<std::string> details;
        std::optional<std::string> state;

        std::optional<ActivityEmoji> emoji;
        std::optional<ActivityParty> party;
        std::optional<ActivityAssets> assets;
        std::optional<ActivitySecrets> secrets;

        std::optional<bool> instance;
        std::optional<int64_t> flags;

        std::optional<std::vector<ActivityButton>> buttons;
};
NC_JSON_DECLFUNCS(Activity, name, type, url, created_at, timestamps, application_id, details, state, emoji, party,
                  assets, secrets, instance, flags, buttons);
