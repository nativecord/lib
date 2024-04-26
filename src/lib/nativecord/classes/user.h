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
    NC_SERIALIZE("id", id);
    NC_SERIALIZE("username", username);
    NC_SERIALIZE_OPT_STR("global_name", globalName);
    NC_SERIALIZE("bot", isBot);
    NC_SERIALIZE("system", isSystem);
    NC_SERIALIZE_OPT_NUM("premium_type", premiumType);
    NC_SERIALIZE_OPT_NUM("public_flags", publicFlags);

    NC_SERIALIZE("avatar", avatar);
    NC_SERIALIZE_OPT_STR("avatar_decoration", avatarDecoration);
    NC_SERIALIZE_OPT_STR("banner", banner);
    NC_SERIALIZE_OPT_NUM("accent_color", accentCol);

    NC_SERIALIZE_OPT_NUM("flags", flags);
    NC_SERIALIZE_OPT("mfa_enabled", mfaEnabled);
    NC_SERIALIZE_OPT("verified", verified);
    NC_SERIALIZE_OPT_STR("email", email);
    NC_SERIALIZE_OPT_NUM("flags", flags); 
    NC_SERIALIZE_OPT_STR("locale", locale);
}

NC_EXPORT inline void from_json(const nlohmann::json& js, User& obj)
{
    NC_DESERIALIZE_UINT64("id", id);
    NC_DESERIALIZE("username", username);
    NC_DESERIALIZE("global_name", globalName);
    NC_DESERIALIZE("bot", isBot);
    NC_DESERIALIZE("system", isSystem);
    NC_DESERIALIZE("premium_type", premiumType);
    NC_DESERIALIZE("public_flags", publicFlags);

    NC_DESERIALIZE("avatar", avatar);
    NC_DESERIALIZE("avatar_decoration", avatarDecoration);
    NC_DESERIALIZE("banner", banner);
    NC_DESERIALIZE("accent_color", accentCol);

    NC_DESERIALIZE("flags", flags);
    NC_DESERIALIZE("mfa_enabled", mfaEnabled);
    NC_DESERIALIZE("verified", verified);
    NC_DESERIALIZE("email", email);
    NC_DESERIALIZE("flags", flags);
    NC_DESERIALIZE("locale", locale);
}