#include "client.h"

#include "nativecord/discord/endpoints.h"
#include "nativecord/discord/enums/gateway_codes.h"
#include "nativecord/discord/objects/guild.h"
#include "nativecord/discord/objects/user.h"

#include "nativecord/classes/websocket_connection.h"

#include "nativecord/http.h"

#include <regex>

#include <libwebsockets.h>

int nativecord::_clientWsCallback(lws* wsi, int reason, void* user, void* in, size_t len)
{
    if (!user)
        return 0;

    int ret = reinterpret_cast<nativecord::Client*>(user)->__wsReceive(wsi, reason, reinterpret_cast<char*>(in), len);
    return ret;
}

nativecord::Client::Client(std::string token) : _token(token)
{
    registerEvents();
    _ws = new WebsocketConnection(NC_WSS_URL, _clientWsCallback, this);
    _guildCache = new Cache<snowflake, Guild>(NC_DEFAULT_GUILD_CACHE_SIZE);
    _userCache = new Cache<snowflake, User>(NC_DEFAULT_USER_CACHE_SIZE);
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
    _dispatchListeners["READY"] = [](Client* client, lws*, nlohmann::json& js) {
        js["d"]["user"].get_to(client->_user);
        client->_emitter.fireEvent("ready", std::forward<Client*>(client));
    };

    //_emitter.registerEvent<Client*, Message*>("message");
    //_dispatchListeners["MESSAGE_CREATE"] = [](Client* client, lws*, nlohmann::json& js) {
    //    Message msg = js["d"];
    //    client->_emitter.
    // fireEvent("message", std::forward<Client*>(client), &msg);
    //};

    _dispatchListeners["GUILD_CREATE"] = [](Client* client, lws*, nlohmann::json& js) {
        auto guild = std::make_unique<Guild>(client);
        js.at("d").get_to(*guild);
        snowflake id = guild->id;
        if (client->getGuildCache()->contains(id))
            return;
        client->getGuildCache()->insert(id, std::move(guild));
    };

    _emitter.registerEvent<uint16_t>("disconnect");
    _emitter.registerEvent<Client*, nlohmann::json&>("dispatch");
}

void nativecord::Client::connect()
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

    _ws->pollEvents();
}

int nativecord::Client::__wsReceive(lws* wsi, int reason, char* in, size_t /*len*/)
{
    switch (reason)
    {
        case LWS_CALLBACK_CLIENT_RECEIVE:
            {
                nlohmann::json js = nlohmann::json::parse(in);
                this->onGateway(wsi, js);
                break;
            }
        case LWS_CALLBACK_TIMER:
            {
                lws_callback_on_writable(wsi);
                break;
            }
        case LWS_CALLBACK_CLIENT_WRITEABLE:
            {
                nlohmann::json heartbeat({{"op", GATEWAY_HEARTBEAT}});
                if (_lastSequence)
                    heartbeat["d"] = _lastSequence;
                wsSendJson(heartbeat);
                lws_set_timer_usecs(wsi, _heartbeatInterval * 1000);
                break;
            }
        case LWS_CALLBACK_WS_PEER_INITIATED_CLOSE:
            {
                uint16_t code;
                memcpy(&code, in, sizeof(uint16_t));
                code = ntohs(code);

                _emitter.fireEvent("disconnect", std::forward<uint16_t>(code));

                return -1;
            }
    }
    return 0;
}

std::unique_ptr<char[]> nativecord::Client::apiCall(const char* path, const char* method, nlohmann::json* payload)
{
    std::string url = std::string(NC_API_URL) + path;
    std::unordered_map<std::string, std::string> headers;

    void* data = nullptr;
    size_t dataSize = 0;

    if (payload)
    {
        std::string payloadString = payload->dump();

        data = malloc(payloadString.size() + 1);
        if (!data)
            throw new std::runtime_error("failed to allocate memory for payload buffer");

        dataSize = payloadString.size();
        memcpy(data, payloadString.c_str(), dataSize);
        reinterpret_cast<char*>(data)[dataSize] = '\0';

        headers["Content-Type"] = "application/json";

    }

    headers["Authorization"] = _token;

    std::unique_ptr<char[]> ret = nativecord::http::request(url.c_str(), method, reinterpret_cast<const char*>(data), dataSize, &headers);
    if (data)
        free(data);
    return ret;
}

void nativecord::Client::onGateway(lws* wsi, nlohmann::json& js)
{
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
                    _dispatchListeners[eventName](this, wsi, js);
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

void nativecord::Client::wsSendJson(nlohmann::json& js)
{
    std::string dmp = js.dump();

    unsigned char* buff = reinterpret_cast<unsigned char*>(malloc(LWS_PRE + dmp.size()));
    if (!buff)
        throw new std::runtime_error("failed to allocate buffer");
    memcpy(buff + LWS_PRE, dmp.c_str(), dmp.size());

    int res = lws_write(_ws->getInterface(), buff + LWS_PRE, dmp.size(), LWS_WRITE_TEXT);
    free(buff);

    if (res < 0)
        throw new std::runtime_error("lws_write failed");
}

void nativecord::Client::identify(nlohmann::json& js)
{
    js["d"]["heartbeat_interval"].get_to(_heartbeatInterval);
    lws_set_timer_usecs(_ws->getInterface(), _heartbeatInterval * 1000);

    nlohmann::json id;
    id["op"] = GATEWAY_IDENTIFY;

    nlohmann::json data({{"token", _token},
                         {"intents", _intents},
                         {"compress", false},
                         {"large_treshold", 250},
                         {"properties", {{"os", "Windows"}, {"browser", "Chrome"}}}});
    id["d"] = data;

    wsSendJson(id);
}
