#pragma once

#include "nativecord/util/macros.h"

#define DISCORD_URL "https://discord.com/"

#define DISCORD_API_VERSION 9
#define DISCORD_API_URL DISCORD_URL "api/v" STR(DISCORD_API_VERSION) "/"

#define DISCORD_WS_URL "wss://gateway.discord.gg/?encoding=json&v=" STR(DISCORD_API_VERSION)
