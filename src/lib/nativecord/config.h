#pragma once

/*
    client cache sizes

    TO-DO:
        This is a temporal workaround, ideally these will be dynamically determined based on bot stats (guild count,
   shard amount, user count, etc)
*/
#ifndef NC_DEFAULT_GUILD_CACHE_SIZE
    #define NC_DEFAULT_GUILD_CACHE_SIZE 512
#endif

#ifndef NC_DEFAULT_CHANNEL_CACHE_SIZE
    #define NC_DEFAULT_CHANNEL_CACHE_SIZE 1024
#endif

#ifndef NC_DEFAULT_USER_CACHE_SIZE
    #define NC_DEFAULT_USER_CACHE_SIZE 2048
#endif

#ifndef NC_DEFAULT_GUILDMEMBER_CACHE_SIZE
    #define NC_DEFAULT_GUILDMEMBER_CACHE_SIZE 1024
#endif