#include "channel_s.h"
#include "guild_s.h"
#include "user_s.h"

#include "nativecord/discord/objects/channel.h"

namespace nativecord
{
    NC_JSON_FUNCS(ThreadMetadata, archived, auto_archive_duration, archive_timestamp, locked, invitable,
                  create_timestamp);
    NC_JSON_FUNCS(ThreadMember, id, user_id, join_timestamp, flags, member);

    NC_JSON_FUNCS(DefaultReaction, emoji_id, emoji_name);
    NC_JSON_FUNCS(Overwrite, id, type, allow, deny);

    NC_JSON_FUNCS(Channel, id, type, guild_id, position, permission_overwrites, name, topic, nsfw, last_message_id,
                  bitrate, user_limit, rate_limit_per_user, recipients, icon, owner_id, application_id, managed,
                  parent_id, last_pin_timestamp, rtc_region, video_quality_mode, message_count, member_count,
                  thread_metadata, member, default_active_duration, permissions, flags, total_message_sent,
                  applied_tags, default_reaction_emoji, default_sort_order, default_thread_rate_limit_per_user,
                  default_forum_layout);
}
