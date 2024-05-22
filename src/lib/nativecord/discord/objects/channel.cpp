#include "channel.h"
#include "message.h"

#include "nativecord/internal/clientbase.h"

using namespace nativecord;

nativecord::Channel::Channel(Client* client) : ObjectBase(client) {}

Channel::Channel(Client* client, snowflake channelId) : ObjectBase(client)
{
    auto res = _getClientBase()->apiCall(std::format("channels/{}", channelId).c_str(), HTTP_GET);
    nlohmann::json channelJs = nlohmann::json::parse(res.second);
    channelJs.get_to(*this);
}

void Channel::sendMessage(std::string content)
{
    nlohmann::json payload({{"content", content}});
    _getClientBase()->apiCall(std::format("channels/{}/messages", id).c_str(), HTTP_POST, &payload);
}

void Channel::sendMessage(Message& msg)
{
    nlohmann::json msgPayload(msg);
    _getClientBase()->apiCall(std::format("channels/{}/messages", id).c_str(), HTTP_POST, &msgPayload);
}
