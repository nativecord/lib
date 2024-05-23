#include "guild.h"
#include "channel.h"

#include "nativecord/config.h"

using namespace nativecord;

Guild::Guild() {}

Guild::Guild(Client* client) : ObjectBase(client) { initCache(); }

Guild::Guild(Client* client, snowflake guildId) : ObjectBase(client)
{
    initCache();
    id = guildId;
}

std::shared_ptr<Guild> Guild::_createShared(Client* client) { return std::shared_ptr<Guild>(new Guild(client)); }

Guild::~Guild()
{
    if (!hasCache)
        return;
    delete _channelCache;
    delete _memberCache;
}

std::shared_ptr<Channel> Guild::getChannel(snowflake channelId)
{
    auto channel = _channelCache->get(channelId);
    return nullptr;
}

std::shared_ptr<GuildMember> Guild::getMember(snowflake memberId)
{
    auto member = _memberCache->get(memberId);
    return nullptr;
}

void nativecord::Guild::initCache()
{
    _channelCache = new Cache<snowflake, Channel>(NC_DEFAULT_CHANNEL_CACHE_SIZE);
    _memberCache = new Cache<snowflake, GuildMember>(NC_DEFAULT_GUILDMEMBER_CACHE_SIZE);
    hasCache = true;
}
