#pragma once

#include "nativecord/util/macros.h"

#include <nlohmann/json.hpp>
#include <string>

enum userStatus
{
    STATUS_ONLINE,
    STATUS_DND,
    STATUS_IDLE,
    STATUS_INVISIBLE,
    STATUS_OFFLINE
};

static const char* userStatusChar[] = {"online", "dnd", "idle", "invisible", "offline"};

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

struct User
{
    public:
        /*
            base info
        */
        uint64_t id = 0;
        std::string username;
        // std::string _discriminator;
        std::string global_name;
        bool bot;
        bool system;
        int premium_type = -1;
        userFlags public_flags = static_cast<userFlags>(-1);

        /*
            appearance
        */
        std::string avatar;
        std::string avatar_decoration;
        std::string banner;
        int accent_color = -1;

        /*
            private info (only for local user)
        */
        userFlags flags;
        bool mfa_enabled;
        bool verified;
        std::string email;
        std::string locale;
};

NC_EXPORT inline void to_json(nlohmann::json& js, const User& obj)
{
    NC_SERIALIZE(id);
    NC_SERIALIZE(username);
    NC_SERIALIZE(global_name);
    NC_SERIALIZE(bot);
    NC_SERIALIZE(system);
    NC_SERIALIZE(premium_type);
    NC_SERIALIZE(public_flags);
    NC_SERIALIZE(avatar);
    NC_SERIALIZE(avatar_decoration);
    NC_SERIALIZE(banner);
    NC_SERIALIZE(accent_color);
    NC_SERIALIZE(flags);
    NC_SERIALIZE(mfa_enabled);
    NC_SERIALIZE(email);
    NC_SERIALIZE(locale);

    NC_SERIALIZE_CLEANUP(js);
}

NC_EXPORT inline void from_json(const nlohmann::json& js, User& obj)
{
    NC_DESERIALIZE_UINT64(id);
    NC_DESERIALIZE(username);
    NC_DESERIALIZE(global_name);
    NC_DESERIALIZE(bot);
    NC_DESERIALIZE(system);
    NC_DESERIALIZE(premium_type);
    NC_DESERIALIZE(public_flags);
    NC_DESERIALIZE(avatar);
    NC_DESERIALIZE(avatar_decoration);
    NC_DESERIALIZE(banner);
    NC_DESERIALIZE(accent_color);
    NC_DESERIALIZE(flags);
    NC_DESERIALIZE(mfa_enabled);
    NC_DESERIALIZE(email);
    NC_DESERIALIZE(locale);
}