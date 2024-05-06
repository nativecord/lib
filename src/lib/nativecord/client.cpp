#include "client.h"

#include "nativecord/discord/endpoints.h"
#include "nativecord/discord/enums/gateway_codes.h"
#include "nativecord/discord/objects/guild.h"
#include "nativecord/discord/objects/message.h"
#include "nativecord/discord/objects/user.h"

#include "nativecord/classes/websocket_connection.h"

#include "nativecord/http.h"

#include <regex>

nativecord::Client::Client(std::string token) : _token(token)
{
    registerEvents();
    _guildCache = new Cache<snowflake, Guild>(NC_DEFAULT_GUILD_CACHE_SIZE);
    _userCache = new Cache<snowflake, User>(NC_DEFAULT_USER_CACHE_SIZE);

    _ws = new WebsocketConnection(NC_WSS_URL, this);

    _ws->on("open", [](WebsocketConnection* conn, std::string) {
        auto client = reinterpret_cast<Client*>(conn->getUserData());
        client->_connected = true;
    });

    _ws->on("message", [](WebsocketConnection* conn, const std::string& msg) {
        auto client = reinterpret_cast<Client*>(conn->getUserData());
        client->__processReceive(msg);
    });

    _ws->on("close", [](WebsocketConnection* conn, int reason) {
        auto client = reinterpret_cast<Client*>(conn->getUserData());
        client->_connected = false;
        client->_emitter.fireEvent("disconnect", client, reason);
    });
}

nativecord::Client::~Client()
{
    delete _ws;
    delete _guildCache;
    delete _userCache;
}

void nativecord::Client::registerEvents()
{
    _emitter.registerEvent<Client*>("ready");
    _dispatchListeners["READY"] = [](Client* client, nlohmann::json& js) {
        js["d"]["user"].get_to(client->_user);
        client->_emitter.fireEvent("ready", std::forward<Client*>(client));
    };

    _emitter.registerEvent<Client*, Message*>("message");
    _dispatchListeners["MESSAGE_CREATE"] = [](Client* client, nlohmann::json& js) {
        Message msg = js["d"];
        client->_emitter.fireEvent("message", std::forward<Client*>(client), &msg);
    };

    _dispatchListeners["GUILD_CREATE"] = [](Client* client, nlohmann::json& js) {
        auto guild = std::make_unique<Guild>(client);
        js.at("d").get_to(*guild);
        snowflake id = guild->id;
        if (client->getGuildCache()->contains(id))
            return;
        client->getGuildCache()->insert(id, std::move(guild));
    };

    _emitter.registerEvent<Client*, int>("disconnect");
    _emitter.registerEvent<Client*, nlohmann::json&>("dispatch");
}

int nativecord::Client::connect()
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

    return _ws->connect();
}

void nativecord::Client::__processReceive(const std::string& msg)
{
    nlohmann::json js = nlohmann::json::parse(msg);

    if (js.contains("s") && !js["s"].is_null())
        js["s"].get_to(_lastSequence);

    gatewayCode code = js["op"];

    switch (code)
    {
        case GATEWAY_DISPATCH:
            {
                _emitter.fireEvent("dispatch", this, js);
                std::string eventName = js["t"].get<std::string>();
                if (_dispatchListeners.contains(eventName))
                    _dispatchListeners[eventName](this, js);
                break;
            }
        case GATEWAY_INVALID_SESSION:
            {
                throw new std::runtime_error("invalid session");
                break;
            }
        case GATEWAY_HELLO:
            {
                identify(js);
                break;
            }
        default:
            break;
    }
}

void nativecord::Client::sendHeartbeat()
{
    nlohmann::json heartbeat({{"op", GATEWAY_HEARTBEAT}});
    if (_lastSequence)
        heartbeat["d"] = _lastSequence;
    sendJson(heartbeat);
}

void nativecord::Client::identify(nlohmann::json& js)
{
    int heartbeatInterval = js["d"]["heartbeat_interval"].get<int>();

    _ws->channel->setHeartbeat(heartbeatInterval, [this]() { this->sendHeartbeat(); });
    // lws_set_timer_usecs(_ws->getInterface(), _heartbeatInterval * 1000);

    nlohmann::json id;
    id["op"] = GATEWAY_IDENTIFY;

    nlohmann::json data({{"token", _token},
                         {"intents", _intents},
                         {"compress", false},
                         {"large_treshold", 250},
                         {"properties", {{"os", "Windows"}, {"browser", "Chrome"}}}});
    id["d"] = data;

    sendJson(id);
}

std::pair<int, std::string> nativecord::Client::apiCall(const char* path, http_method method, nlohmann::json* payload)
{
    std::string url = std::string(NC_API_URL) + path;
    std::unordered_map<std::string, std::string> headers;

    std::optional<std::string> body = std::nullopt;

    if (payload)
    {
        body = payload->dump();
        headers["Content-Type"] = "application/json";
    }
    headers["Authorization"] = _token;

    return nativecord::http::req(url, method, body, &headers);
}

void nativecord::Client::sendJson(nlohmann::json& js)
{
    if (!_ws->isConnected())
        return;
    std::string jsStr = js.dump();
    _ws->send(jsStr);
}