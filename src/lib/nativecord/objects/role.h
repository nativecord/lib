#pragma once

#include "nativecord/util/jsonutils.h"

#include <stdint.h>
#include <string>

struct RoleTags
{
        std::optional<uint64_t> bot_id;         // optional
        std::optional<uint64_t> integration_id; // optional

        // null premium_subscriber; // ??? (this is documented as type: null?)
        // null available_for_purchase; // same as above
        // null guild_connections; // same as above...

        // uint64_t subscription_listing_id; // this doesn't seem to be working?
};
NC_JSON_DECLFUNCS(RoleTags, bot_id, integration_id);

struct Role
{
        uint64_t id;
        std::string name;
        int color;
        bool hoist;

        std::optional<std::string> icon;
        std::optional<std::string> unicode_emoji;

        int position;
        std::string permissions;
        bool managed;
        bool mentionable;
        std::optional<RoleTags> tags;
};
NC_JSON_DECLFUNCS(Role, id, name, color, hoist, icon, unicode_emoji, position, permissions, managed, mentionable, tags);