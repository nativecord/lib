#pragma once

#include "nativecord/objects/user.h"
#include "nativecord/util/jsonutils.h"

#include <optional>
#include <stdint.h>
#include <string>
#include <vector>

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