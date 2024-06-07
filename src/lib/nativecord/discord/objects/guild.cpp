#include "guild.h"
#include "channel.h"

#include "nativecord/config.h"

#include "nativecord/internal/clientbase.h"

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

NC_EXPORT void nativecord::Guild::populateChannels()
{
    auto res = _getClientBase()->apiCall(std::format("guilds/{}/channels", id).c_str(), HTTP_GET);

    try
    {
        nlohmann::json channelsJS = nlohmann::json::parse(res.second);
        for (auto channelJS : channelsJS)
        {
            auto channel = Channel::_createShared(_client);
            channelJS.get_to(*channel);
            _channelCache->insert(channel->id, channel);
        }
    }
    catch (std::exception)
    {
        return;
    }
}

void nativecord::Guild::initCache()
{
    _channelCache = new Cache<snowflake, Channel>(NC_DEFAULT_CHANNEL_CACHE_SIZE);
    _memberCache = new Cache<snowflake, GuildMember>(NC_DEFAULT_GUILDMEMBER_CACHE_SIZE);
    hasCache = true;
}
