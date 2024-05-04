#pragma once

#include "nativecord/util/macros.h"

#define NC_DISCORD "https://discord.com"

#define NC_API_VERSION 9
#define NC_API_URL NC_DISCORD "/api/v" STR(NC_API_VERSION) "/"
#define NC_WSS_URL "wss://gateway.discord.gg/?encoding=json&v=" STR(NC_API_VERSION)
