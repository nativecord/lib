#pragma once

#include "nativecord/util/macros.h"

#include "nativecord/objects/guild.h"
#include "nativecord/objects/message.h"
#include "nativecord/objects/misc.h"
#include "nativecord/objects/user.h"

#include <nlohmann/json.hpp>
#include <stdint.h>
#include <string>
#include <vector>

enum channelType : int
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
    public:
        bool archived;
        int auto_archive_duration;
        std::string archive_timestamp;
        bool locked;
        bool invitable;               // optional
        std::string create_timestamp; // optional
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ThreadMetadata, archived, auto_archive_duration, archive_timestamp,
                                                locked, invitable, create_timestamp);

struct ThreadMember
{
    public:
        uint64_t id; // optional
        uint64_t user_id;
        std::string join_timestamp; // optional
        int flags;                  // ?
        GuildMember member;         // optional
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ThreadMember, id, user_id, join_timestamp, flags, member);

struct DefaultReaction
{
    public:
        uint64_t emoji_id;      // optional val
        std::string emoji_name; // optional val
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(DefaultReaction, emoji_id, emoji_name);

struct Channel
{
    public:
        uint64_t id;
        channelType type;
        uint64_t guild_id;                            // optional
        int position;                                 // optional
        std::vector<Overwrite> permission_overwrites; // optional
        std::string name;                             // optional
        std::string topic;                            // optional
        bool nsfw;                                    // optional
        uint64_t last_message_id;                     // optional
        int bitrate;                                  // optional
        int user_limit;                               // optional
        int rate_limit_per_user;                      // optional
        std::vector<User> recipients;                 // optional
        std::string icon;                             // optional
        uint64_t owner_id;                            // optional
        uint64_t application_id;                      // optional
        bool managed;                                 // optional
        uint64_t parent_id;                           // optional
        std::string last_pin_timestamp;               // optional
        std::string rtc_region;                       // optional
        int video_quality_mode;                       // optional
        int message_count;                            // optional
        int member_count;                             // optional
        ThreadMetadata thread_metadata;               // optional
        ThreadMember member;                          // optional
        int default_active_duration;                  // optional
        std::string permissions;                      // optional
        channelFlags flags;                           // optional
        int total_message_sent;                       // optional (no, its not a typo, discord spelt it "message")
        std::vector<uint64_t> applied_tags;           // optional
        DefaultReaction default_reaction_emoji;       // optional
        int default_thread_rate_limit_per_user;       // optional
        int default_sort_order;                       // optional
        int default_forum_layout;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Channel, id, type, guild_id, position, permission_overwrites, name,
                                                topic, nsfw, last_message_id, bitrate, user_limit, rate_limit_per_user,
                                                recipients, icon, owner_id, application_id, managed, parent_id,
                                                last_pin_timestamp, rtc_region, video_quality_mode, message_count,
                                                member_count, thread_metadata, member, default_active_duration,
                                                permissions, flags, total_message_sent, applied_tags,
                                                default_thread_rate_limit_per_user, default_sort_order,
                                                default_forum_layout);
