#include "user.h"

#include "nativecord/discord/objects/user.h"

namespace nativecord
{
    NC_JSON_FUNCS(User, id, username, global_name, bot, system, premium_type, public_flags, avatar, avatar_decoration,
                  banner, accent_color, flags, mfa_enabled, verified, email, locale);
}
