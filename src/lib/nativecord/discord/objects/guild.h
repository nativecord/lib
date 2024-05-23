#pragma once

#include "nativecord/internal/objectbase.h"
#include "nativecord/internal/serializers/guild.h"

#include "nativecord/discord/defs.h"

#include "nativecord/util/cache.h"

#include "user.h"

#include <memory>

namespace nativecord
{
    struct WelcomeScreenChannel
    {
            snowflake channel_id;
            std::string description;
            std::optional<uint64_t> emoji_id;
            std::optional<std::string> emoji_name;
    };

    struct WelcomeScreen
    {
            std::optional<std::string> description;
            std::vector<WelcomeScreenChannel> welcome_channels;
    };

    struct AvatarDecorationData
    {
            std::string asset;
            std::string sku_id;
    };

    struct GuildMember
    {
            std::optional<User> user;
            std::optional<std::string> nick;
            std::optional<std::string> avatar;
            std::vector<uint64_t> roles;
            std::string joined_at;
            std::optional<std::string> premium_since;
            bool deaf;
            bool mute;
            uint64_t flags;
            std::optional<bool> pending;
            std::optional<std::string> permissions;
            std::optional<std::string> communication_disabled_until;
            std::optional<AvatarDecorationData> avatar_decoration_data;
    };

    class Guild : ObjectBase
    {
        public:
            NC_EXPORT Guild(Client* client, snowflake id);
            ~Guild();

            NC_EXPORT std::shared_ptr<class Channel> getChannel(snowflake id);
            NC_EXPORT std::shared_ptr<struct GuildMember> getMember(snowflake id);

            NC_EXPORT auto getChannelCache() { return _channelCache; }

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

            friend void from_json(const nlohmann::json& nlohmann_json_j, Guild& nlohmann_json_t);

        protected:
            friend class ClientBase;

            Guild();
            Guild(Client* client);

            static std::shared_ptr<Guild> _createShared(Client*);

        private:
            void initCache();
            bool hasCache = false;

            Cache<snowflake, Channel>* _channelCache;
            Cache<snowflake, GuildMember>* _memberCache;
    };
}
