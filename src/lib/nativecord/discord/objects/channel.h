#pragma once

#include "objects.h"

#include "user.h"

enum channelType : int32_t
{
    CHANNELTYPE_GUILD_TEXT,
    CHANNELTYPE_DM,
    CHANNELTYPE_GUILD_VOICE,
    CHANNELTYPE_GROUP_DM,
    CHANNELTYPE_GUILD_CATEGORY,
    CHANNELTYPE_GUILD_ANNOUNCEMENT,
    CHANNELTYPE_ANNOUNCEMENT_THREAD,
    CHANNELTYPE_PUBLIC_THREAD,
    CHANNELTYPE_PRIVATE_THREAD,
    CHANNELTYPE_GUILD_STAGE_VOICE,
    CHANNELTYPE_GUILD_DIRECTORY,
    CHANNELTYPE_GUILD_FORUM,
    CHANNELTYPE_GUILD_MEDIA
};

enum channelFlags
{
    CHANNELFLAG_PINNED = 1 << 1,
    CHANNELFLAG_REQUIRE_TAG = 1 << 4,
    CHANNELFLAG_HIDE_MEDIA_DOWNLOAD_OPTIONS = 1 << 15,
};

struct ThreadMetadata
{
        bool archived;
        int auto_archive_duration;
        std::string archive_timestamp;
        bool locked;
        std::optional<bool> invitable;
        std::optional<std::string> create_timestamp;
};
NC_JSON_DECLFUNCS(ThreadMetadata, archived, auto_archive_duration, archive_timestamp, locked, invitable,
                  create_timestamp);

struct ThreadMember
{
        std::optional<snowflake> id;
        snowflake user_id;
        std::optional<std::string> join_timestamp;
        int flags;
        std::optional<GuildMember> member;
};
NC_JSON_DECLFUNCS(ThreadMember, id, user_id, join_timestamp, flags, member);

struct DefaultReaction
{
        std::optional<snowflake> emoji_id;
        std::optional<std::string> emoji_name;
};
NC_JSON_DECLFUNCS(DefaultReaction, emoji_id, emoji_name);

struct Overwrite
{
        snowflake id;
        int type;
        std::string allow;
        std::string deny;
};
NC_JSON_DECLFUNCS(Overwrite, id, type, allow, deny);

class Channel : ObjectBase
{
    public:
        using ObjectBase::ObjectBase;
        NC_EXPORT Channel(nativecord::Client* client, snowflake channelId);

        NC_EXPORT void sendMessage(std::string content);
        NC_EXPORT void sendMessage(class Message* msg);

        snowflake id;
        channelType type;

        std::optional<snowflake> guild_id;
        std::optional<int> position;
        std::optional<std::vector<Overwrite>> permission_overwrites;
        std::optional<std::string> name;
        std::optional<std::string> topic;
        std::optional<bool> nsfw;
        std::optional<snowflake> last_message_id;
        std::optional<int> bitrate;
        std::optional<int> user_limit;
        std::optional<int> rate_limit_per_user;
        std::optional<std::vector<User>> recipients;
        std::optional<std::string> icon;
        std::optional<snowflake> owner_id;
        std::optional<snowflake> application_id;
        std::optional<bool> managed;
        std::optional<snowflake> parent_id;
        std::optional<std::string> last_pin_timestamp;
        std::optional<std::string> rtc_region;
        std::optional<int> video_quality_mode;
        std::optional<int> message_count;
        std::optional<int> member_count;
        std::optional<ThreadMetadata> thread_metadata;
        std::optional<ThreadMember> member;
        std::optional<int> default_active_duration;
        std::optional<std::string> permissions;
        std::optional<channelFlags> flags;
        std::optional<int> total_message_sent;
        std::optional<std::vector<uint64_t>> applied_tags;
        std::optional<DefaultReaction> default_reaction_emoji;
        std::optional<int> default_sort_order;
        std::optional<int> default_thread_rate_limit_per_user;
        std::optional<int> default_forum_layout;
};
NC_JSON_DECLFUNCS(Channel, id, type, guild_id, position, permission_overwrites, name, topic, nsfw, last_message_id,
                  bitrate, user_limit, rate_limit_per_user, recipients, icon, owner_id, application_id, managed,
                  parent_id, last_pin_timestamp, rtc_region, video_quality_mode, message_count, member_count,
                  thread_metadata, member, default_active_duration, permissions, flags, total_message_sent,
                  applied_tags, default_reaction_emoji, default_sort_order, default_thread_rate_limit_per_user,
                  default_forum_layout);
