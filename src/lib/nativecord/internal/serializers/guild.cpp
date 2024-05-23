#include "guild.h"
#include "channel.h"

#include "nativecord/discord/objects/guild.h"
#include "nativecord/discord/objects/channel.h"

namespace nativecord
{
    NC_JSON_FUNCS(AvatarDecorationData, asset, sku_id)
    NC_JSON_FUNCS(GuildMember, nick, avatar, roles, joined_at, premium_since, deaf, mute, flags, pending, permissions,
                  communication_disabled_until, avatar_decoration_data);
    NC_JSON_FUNCS(WelcomeScreenChannel, channel_id, description, emoji_id, emoji_name);
    NC_JSON_FUNCS(WelcomeScreen, description, welcome_channels);

    inline void to_json(nlohmann::json& nlohmann_json_j, const Guild& nlohmann_json_t)
    {
        NLOHMANN_JSON_PASTE(NC_TO_JSON, id, name, icon, icon_hash, splash, discovery_splash, owner, owner_id,
                            permissions, verification_level, default_message_notifications, explicit_content_filter,
                            nsfw_level, mfa_level, widget_channel_id, widget_enabled, application_id,
                            public_updates_channel_id, rules_channel_id, safety_alerts_channel_id, system_channel_id,
                            system_channel_flags, afk_channel_id, afk_timeout, vanity_url_code, description, banner,
                            premium_tier, premium_subscription_count, premium_bar_enabled, preferred_locale,
                            max_members, max_presences, max_video_channel_users, max_stage_video_channel_users,
                            approximate_member_count, approximate_presence_count, welcome_screen);
    }

    inline void from_json(const nlohmann::json& nlohmann_json_j, Guild& nlohmann_json_t)
    {
         if (nlohmann_json_j.contains("channels"))
        {
             for (auto channelJs : nlohmann_json_j["channels"])
             {
                 auto channel = Channel::_createShared(nlohmann_json_t._getClient());
                 channelJs.get_to(*channel);
                 std::string id = channel->id;
                 nlohmann_json_t.getChannelCache()->insert(id, std::move(channel));
             }
         }

        NLOHMANN_JSON_PASTE(NC_FROM_JSON, id, name, icon, icon_hash, splash, discovery_splash, owner, owner_id,
                            permissions, verification_level, default_message_notifications, explicit_content_filter,
                            nsfw_level, mfa_level, widget_channel_id, widget_enabled, application_id,
                            public_updates_channel_id, rules_channel_id, safety_alerts_channel_id, system_channel_id,
                            system_channel_flags, afk_channel_id, afk_timeout, vanity_url_code, description, banner,
                            premium_tier, premium_subscription_count, premium_bar_enabled, preferred_locale,
                            max_members, max_presences, max_video_channel_users, max_stage_video_channel_users,
                            approximate_member_count, approximate_presence_count, welcome_screen);
    }
}
