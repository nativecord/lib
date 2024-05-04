#pragma once

#include "objects.h"

#include "nativecord/classes/cache.h"

#define GUILD_CHANNEL_CACHE_SIZE 250

struct WelcomeScreenChannel
{
        uint64_t channel_id;
        std::string description;
        std::optional<uint64_t> emoji_id;
        std::optional<std::string> emoji_name;
};
NC_JSON_DECLFUNCS(WelcomeScreenChannel, channel_id, description, emoji_id, emoji_name);

struct WelcomeScreen
{
        std::optional<std::string> description;
        std::vector<WelcomeScreenChannel> welcome_channels;
};
NC_JSON_DECLFUNCS(WelcomeScreen, description, welcome_channels);

class Guild : public ObjectBase
{
    public:
        Guild();
        Guild(nativecord::Client* client);
        ~Guild();

        inline Cache<snowflake, class Channel>* getChannelCache() const { return _channelCache; }

        snowflake id;
        std::string name;
        std::optional<std::string> icon;
        std::optional<std::string> icon_hash;
        std::optional<std::string> splash;
        std::optional<std::string> discovery_splash;

        std::optional<bool> owner;
        snowflake owner_id;

        std::optional<std::string> permissions;

        int verification_level;
        int default_message_notifications;
        int explicit_content_filter;
        int nsfw_level;
        int mfa_level;

        // std::vector<Role> roles;
        // std::vector<Emoji> emojis;
        // std::vector<GuildFeature> features;
        // std::vector<Sticker> stickers;

        std::optional<snowflake> widget_channel_id;
        std::optional<bool> widget_enabled;
        std::optional<snowflake> application_id;
        std::optional<snowflake> public_updates_channel_id;
        std::optional<snowflake> rules_channel_id;
        std::optional<snowflake> safety_alerts_channel_id;
        std::optional<snowflake> system_channel_id;
        int system_channel_flags;
        std::optional<snowflake> afk_channel_id;
        std::optional<int> afk_timeout;

        std::optional<std::string> vanity_url_code;
        std::optional<std::string> description;
        std::optional<std::string> banner;

        int premium_tier;
        std::optional<int> premium_subscription_count;
        bool premium_bar_enabled;

        std::string preferred_locale;

        std::optional<int> max_members;
        std::optional<int> max_presences;
        std::optional<int> max_video_channel_users;
        std::optional<int> max_stage_video_channel_users;
        std::optional<int> approximate_member_count;
        std::optional<int> approximate_presence_count;

        std::optional<WelcomeScreen> welcome_screen;

    private:
        Cache<snowflake, class Channel>* _channelCache;
};
void to_json(nlohmann::json& nlohmann_json_j, const Guild& nlohmann_json_t);
void from_json(const nlohmann::json& nlohmann_json_j, Guild& nlohmann_json_t);