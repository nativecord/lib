#pragma once

#include "objects.h"

enum userStatus
{
    STATUS_ONLINE,
    STATUS_DND,
    STATUS_IDLE,
    STATUS_INVISIBLE,
    STATUS_OFFLINE
};
NLOHMANN_JSON_SERIALIZE_ENUM(userStatus, {
                                             {STATUS_ONLINE, "online"},
                                             {STATUS_DND, "dnd"},
                                             {STATUS_IDLE, "idle"},
                                             {STATUS_INVISIBLE, "invisible"},
                                             {STATUS_OFFLINE, "offline"},
                                         });

enum userFlags : int64_t
{
    FLAG_STAFF = 1 << 0,
    FLAG_PARTNER = 1 << 1,
    FLAG_HYPESQUAD = 1 << 2,
    FLAG_BUG_HUNTER_LEVEL_1 = 1 << 3,
    FLAG_MFA_SMS = 1 << 4,                 // private
    FLAG_PREMIUM_PROMO_DISMISSED = 1 << 5, // private
    FLAG_HYPESQUAD_ONLINE_HOUSE_1 = 1 << 6,
    FLAG_HYPESQUAD_ONLINE_HOUSE_2 = 1 << 7,
    FLAG_HYPESQUAD_ONLINE_HOUSE_3 = 1 << 8,
    FLAG_PREMIUM_EARLY_SUPPORTER = 1 << 9,
    FLAG_TEAM_USER = 1 << 10,
    FLAG_INTERNAL_APPLICATION = 1 << 11, // private
    FLAG_SYSTEM = 1 << 12,
    FLAG_HAS_UNREAD_URGENT_MESSAGES = 1 << 13, // private
    FLAG_BUG_HUNTER_LEVEL_2 = 1 << 14,
    FLAG_UNDERAGE_DELETED = 1 << 15, // private
    FLAG_VERIFIED_BOT = 1 << 16,
    FLAG_VERIFIED_DEVELOPER = 1 << 17,
    FLAG_CERTIFIED_MODERATOR = 1 << 18,
    FLAG_BOT_HTTP_INTERACTIONS = 1 << 19,
    FLAG_SPAMMER = 1 << 20,
    FLAG_DISABLE_PREMIUM = 1 << 21, // private
    FLAG_ACTIVE_DEVELOPER = 1 << 22,
};

class User : ObjectBase
{
    public:
        snowflake id;
        std::string username;
        // std::string discriminator; // deprecated
        std::optional<std::string> global_name;
        std::optional<bool> bot;
        std::optional<bool> system;
        std::optional<int> premium_type;
        std::optional<userFlags> public_flags;
        std::optional<std::string> avatar;
        std::optional<std::string> avatar_decoration;
        std::optional<std::string> banner;
        std::optional<int> accent_color;
        std::optional<userFlags> flags;
        std::optional<bool> mfa_enabled;
        std::optional<bool> verified;
        std::optional<std::string> email;
        std::optional<std::string> locale;
};
NC_JSON_DECLFUNCS(User, id, username, global_name, bot, system, premium_type, public_flags, avatar, avatar_decoration,
                  banner, accent_color, flags, mfa_enabled, verified, email, locale);

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
};
NC_JSON_DECLFUNCS(GuildMember, nick, avatar, roles, joined_at, premium_since, deaf, mute, flags, pending, permissions,
                  communication_disabled_until);