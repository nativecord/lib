#pragma once

#include "nativecord/discord/defs.h"

#include "nativecord/internal/objectbase.h"
#include "serializers/user_s.h"

#include <optional>

namespace nativecord
{
    enum UserStatus
    {
        STATUS_ONLINE,
        STATUS_DND,
        STATUS_IDLE,
        STATUS_INVISIBLE,
        STATUS_OFFLINE
    };

    enum UserFlags : int64_t
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

    class User
    {
        public:
            snowflake id;
            std::string username;
            // std::string discriminator; // deprecated
            std::optional<std::string> global_name;
            std::optional<bool> bot;
            std::optional<bool> system;
            std::optional<int> premium_type;
            std::optional<UserFlags> public_flags;
            std::optional<std::string> avatar;
            std::optional<std::string> avatar_decoration;
            std::optional<std::string> banner;
            std::optional<int> accent_color;
            std::optional<UserFlags> flags;
            std::optional<bool> mfa_enabled;
            std::optional<bool> verified;
            std::optional<std::string> email;
            std::optional<std::string> locale;
    };
}