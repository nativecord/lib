#include "channel.h"
#include "message.h"

#include "nativecord/client.h"

#include <format>

Channel::Channel(nativecord::Client* client, snowflake channelId)
{
    _client = client;
    auto res = _client->apiCall(std::format("channels/{}", channelId).c_str(), "GET");
    nlohmann::json channelJs = nlohmann::json::parse(res.get());
    channelJs.get_to(*this);
}

void Channel::sendMessage(std::string content)
{
    nlohmann::json payload({{"content", content}});
    _client->apiCall(std::format("channels/{}/messages", id).c_str(), "POST", &payload);
}

void Channel::sendMessage(Message* msg)
{
    nlohmann::json msgPayload(*msg);
    _client->apiCall(std::format("channels/{}/messages", id).c_str(), "POST", &msgPayload);
}
