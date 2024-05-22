#include "clientbase.h"

#include "nativecord/config.h"

#include "nativecord/discord/endpoints.h"
#include "nativecord/discord/enums/gateway_codes.h"

#include "nativecord/discord/objects/guild.h"
#include "nativecord/discord/objects/message.h"

#include "nativecord/util/http.h"
#include "nativecord/util/websocket_connection.h"

#include <regex>

nativecord::ClientBase::ClientBase(std::string token) : _ws(DISCORD_WS_URL, true, this)
{
    _guildCache = new Cache<snowflake, Guild>(NC_DEFAULT_GUILD_CACHE_SIZE);
    _channelCache = new Cache<snowflake, Channel>(NC_DEFAULT_CHANNEL_CACHE_SIZE);
    _userCache = new Cache<snowflake, User>(NC_DEFAULT_USER_CACHE_SIZE);

    _ws.on("message", [](WebsocketConnection* conn, const std::string& msg) {
        auto client = reinterpret_cast<ClientBase*>(conn->getUserData());
        client->gatewayReceive(msg);
    });

    _ws.on("close", [](WebsocketConnection* conn) {
        auto client = reinterpret_cast<ClientBase*>(conn->getUserData());
        client->_ev.fireEvent("disconnect", reinterpret_cast<Client*>(client));
    });

    registerEvents();
}

nativecord::ClientBase::~ClientBase()
{
    delete _guildCache;
    delete _channelCache;
    delete _userCache;
}

void nativecord::ClientBase::registerEvents()
{
    _ev.registerEvent<Client*, Message*>("message");
    _dispatchListeners["MESSAGE_CREATE"] = [](ClientBase* client, nlohmann::json& js) {
        Message msg = js["d"];
        client->_ev.fireEvent("message", std::forward<Client*>(reinterpret_cast<Client*>(client)), &msg);
    };

    _ev.registerEvent<Client*>("ready");
    _dispatchListeners["READY"] = [](ClientBase* client, nlohmann::json& js) {
        js["d"]["user"].get_to(client->_user);
        client->_ev.fireEvent("ready", std::forward<Client*>(reinterpret_cast<Client*>(client)));
    };

    _dispatchListeners["GUILD_CREATE"] = [](ClientBase* client, nlohmann::json& js) {
        auto guild = std::make_shared<Guild>(reinterpret_cast<Client*>(client));
        js.at("d").get_to(*guild);
        snowflake id = guild->id;
        if (client->_guildCache->contains(id))
            return;
        client->_guildCache->insert(id, std::move(guild));
    };

    _ev.registerEvent<Client*>("disconnect");
    _ev.registerEvent<Client*, std::string, nlohmann::json&>("dispatch");
    _ev.registerEvent<Client*, std::string>("error");
}

void nativecord::ClientBase::gatewayConnect()
{
    // token / intent validation
    if (_token.empty())
        throw new std::logic_error("token not set");
    if (_intents == 0)
        throw new std::logic_error("intents not set");

    bool botPrefix = false;
    if (_token.compare(0, 4, "Bot ") == 0)
        botPrefix = true;
    std::regex tokenRegex(
        "^(mfa\\.[a-zA-Z0-9_-]{20,})|([a-zA-Z0-9_-]{23,28}\\.[a-zA-Z0-9_-]{6,7}\\.[a-zA-Z0-9_-]{27,40})$");

    if (!std::regex_match(botPrefix ? _token.substr(4) : _token, tokenRegex))
        throw new std::logic_error("invalid token");

    _ws.connect();
}

void nativecord::ClientBase::gatewayHeartbeat()
{
    nlohmann::json heartbeat({{"op", GATEWAY_HEARTBEAT}});
    if (_lastSequence)
        heartbeat["d"] = _lastSequence;
    gatewaySend(heartbeat);
}

void nativecord::ClientBase::gatewayHello(nlohmann::json& js)
{
    int heartbeatInterval = js["d"]["heartbeat_interval"].get<int>();
    _ws.channel->setHeartbeat(heartbeatInterval, [this]() { this->gatewayHeartbeat(); });

    nlohmann::json id;
    id["op"] = GATEWAY_IDENTIFY;

    nlohmann::json data({{"token", _token},
                         {"intents", _intents},
                         {"compress", false},
                         {"large_treshold", 250},
                         {"properties", {{"os", "Windows"}, {"browser", "Chrome"}}}});
    id["d"] = data;

    gatewaySend(id);
}

void nativecord::ClientBase::gatewaySend(nlohmann::json& js)
{
    std::string jsStr = js.dump();
    _ws.send(jsStr);
}

std::pair<int, std::string> nativecord::ClientBase::apiCall(const char* path, http_method method,
                                                            nlohmann::json* payload)
{
    std::string url = std::string(DISCORD_API_URL) + path;
    std::unordered_map<std::string, std::string> headers;

    std::optional<std::string> body = std::nullopt;

    if (payload)
    {
        body = payload->dump();
        headers["Content-Type"] = "application/json";
    }
    headers["Authorization"] = _token;

    return http::req(url, method, body, &headers);
}

void nativecord::ClientBase::gatewayReceive(const std::string& data)
{
    nlohmann::json js = nlohmann::json::parse(data);

    if (js.contains("s") && !js["s"].is_null())
        js["s"].get_to(_lastSequence);

    GatewayCode code = js["op"];
    switch (code)
    {
        case GATEWAY_DISPATCH:
            {
                _ev.fireEvent("dispatch", reinterpret_cast<Client*>(this), js["t"].get<std::string>(), js["d"]);
                std::string eventName = js["t"].get<std::string>();
                if (_dispatchListeners.contains(eventName))
                    _dispatchListeners[eventName](this, js);
                break;
            }
        case GATEWAY_INVALID_SESSION:
            {
                _ev.fireEvent("error", reinterpret_cast<Client*>(this), "Invalid session");
                break;
            }
        case GATEWAY_HELLO:
            {
                gatewayHello(js);
                break;
            }
    }
}
