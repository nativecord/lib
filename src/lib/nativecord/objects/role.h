#pragma once

#include "nativecord/util/macros.h"

#include <stdint.h>
#include <string>

#include <nlohmann/json.hpp>

struct RoleTags
{
    public:
        uint64_t bot_id = 0; // optional
        uint64_t integration_id = 0; // optional

        //null premium_subscriber; // ??? (this is documented as type: null?)
        //null available_for_purchase; // same as above
        //null guild_connections; // same as above...
      
        //uint64_t subscription_listing_id; // this doesn't seem to be working?
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(RoleTags, bot_id, integration_id);

struct Role
{
    public:
        uint64_t id;
        std::string name;
        int color;
        bool hoist;
        
        std::string icon; // optional
        std::string unicode_emoji; // optional

        int position;
        std::string permissions;
        bool managed;
        bool mentionable;
        RoleTags tags; // optional

        //int32_t flags; (this doesn't seem that important?)
};

NC_EXPORT inline void to_json(nlohmann::json& js, const Role& obj)
{
    NC_SERIALIZE(id);
    NC_SERIALIZE(name);
    NC_SERIALIZE(color);
    NC_SERIALIZE(hoist);
    NC_SERIALIZE(icon);
    NC_SERIALIZE(unicode_emoji);
    NC_SERIALIZE(position);
    NC_SERIALIZE(managed);
    NC_SERIALIZE(mentionable);
    NC_SERIALIZE(tags);
    //NC_SERIALIZE(flags);

    NC_SERIALIZE_CLEANUP(js);
}

NC_EXPORT inline void from_json(const nlohmann::json& js, Role& obj)
{
    NC_DESERIALIZE_UINT64(id);
    NC_DESERIALIZE(name);
    NC_DESERIALIZE(color);
    NC_DESERIALIZE(hoist);
    NC_DESERIALIZE(icon);
    NC_DESERIALIZE(unicode_emoji);
    NC_DESERIALIZE(position);
    NC_DESERIALIZE(permissions);
    NC_DESERIALIZE(managed);
    NC_DESERIALIZE(mentionable);
    NC_DESERIALIZE(tags);
    //NC_DESERIALIZE(flags);
}