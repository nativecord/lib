#include "client.h"

#include "nativecord/discord/objects/channel.h"

using namespace nativecord;

Client::Client() : ClientBase("") {}

Client::Client(std::string token) : ClientBase(token) {}

Client::~Client() {}

NC_EXPORT void Client::connect() { gatewayConnect(); }

NC_EXPORT const User* Client::getUser() const { return &_user; }

inline void Client::setToken(std::string token) { _token = token; }

inline std::string Client::getToken() { return _token; }

inline void Client::setIntents(ClientIntents intents) { _intents = intents; }

inline ClientIntents Client::getIntents() { return _intents; }

NC_EXPORT std::shared_ptr<Channel> nativecord::Client::getChannel(std::string id)
{
    std::shared_ptr<Channel> channel = _channelCache->get(id);
    if (channel != nullptr)
        return channel;

    channel = std::make_shared<Channel>(this, id);
    _channelCache->insert(id, channel);

    return channel;
}

NC_EXPORT std::shared_ptr<Guild> nativecord::Client::getGuild(std::string id)
{
    std::shared_ptr<Guild> guild = _guildCache->get(id);
    if (guild != nullptr)
        return guild;

    guild = std::make_shared<Guild>(this, id);
    _guildCache->insert(id, guild);

    return guild;
}
