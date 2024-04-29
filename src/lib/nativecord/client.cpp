#include "client.h"
#include "endpoints.h"
#include "net.h"

#include "objects/message.h"

#include <regex>
#include <stdexcept>

#include <libwebsockets.h>

#include <nlohmann/json.hpp>

inline nativecord::Client::Client(std::string token) : _token(token)
{
    registerEvents();
}

void nativecord::Client::identify(nlohmann::json& js)
{
    js["d"]["heartbeat_interval"].get_to(_heartbeatInterval);
    lws_set_timer_usecs(_wsi, _heartbeatInterval * 1000);

    nlohmann::json id;
    id["op"] = GATEWAY_IDENTIFY;

    nlohmann::json data({{"token", _token},
                         {"intents", _intents},
                         {"compress", false},
                         {"large_treshold", 250},
                         {"properties", {{"$os", "Windows"}, {"$browser", "Chrome"}}}});
    id["d"] = data;

    sendJson(id);
}

void nativecord::Client::registerEvents()
{
    _emitter.registerEvent<Client*>("ready");
    _dispatchListeners["READY"] = [](Client* client, lws*, nlohmann::json& js) {
        js["d"]["user"].get_to(client->_user);
        client->_emitter.fireEvent("ready", std::forward<Client*>(client));
    };

    _emitter.registerEvent<Client*, Message*>("message");
    _dispatchListeners["MESSAGE_CREATE"] = [](Client* client, lws*, nlohmann::json& js) {
        Message msg = js["d"];
        client->_emitter.fireEvent("message", std::forward<Client*>(client), &msg);
    };

    _emitter.registerEvent<uint16_t>("disconnect");
    _emitter.registerEvent<Client*, lws*, nlohmann::json&>("dispatch");
}

void nativecord::Client::sendJson(nlohmann::json& js)
{
    if (_wsi == nullptr)
        throw new std::logic_error("called sendJson outside of a valid callback");

    std::string dump = js.dump();
    unsigned char* buff = reinterpret_cast<unsigned char*>(malloc(LWS_PRE + dump.size()));
    if (!buff)
        throw new std::runtime_error("failed to allocate buffer");
    memcpy(buff + LWS_PRE, dump.c_str(), dump.size());

    int res = lws_write(_wsi, buff + LWS_PRE, dump.size(), LWS_WRITE_TEXT);
    free(buff);
    if (res < 0)
        throw new std::runtime_error("lws_write failed");
}

NC_EXPORT void nativecord::Client::connect()
{
    // token / intent validation
    if (_token.empty())
        throw new std::logic_error("token not set");
    if (_intents == 0)
        throw new std::logic_error("intents not set");

    bool botPrefix = false;
    if (_token.compare(0, 4, "Bot ") == 0)
        botPrefix = true;
    std::regex tokenRegex("^(mfa\\.[a-zA-Z0-9_-]{20,})|([a-zA-Z0-9_-]{23,28}\\.[a-zA-Z0-9_-]{6,7}\\.[a-zA-Z0-9_-]{27,40})$");
    
    if (!std::regex_match(botPrefix ? _token.substr(4) : _token, tokenRegex))
        throw new std::logic_error("invalid token");

    // websocket connection
    lws_context* ctx = nativecord::net::getContext();

    char* wss = reinterpret_cast<char*>(malloc(strlen(NC_WSS_URL) + 1));
    ASSERT(wss, "failed to allocate memory for wss uri");
    memcpy(wss, NC_WSS_URL, strlen(NC_WSS_URL));
    wss[strlen(NC_WSS_URL)] = '\0';

    lws_client_connect_info connInfo{};

    if (lws_parse_uri(wss, &connInfo.protocol, &connInfo.address, &connInfo.port, &connInfo.path))
        throw new std::exception("failed to parse wss uri");

    lws* wsi = nullptr;
    connInfo.pwsi = &wsi;
    connInfo.context = ctx;
    connInfo.host = connInfo.address;
    connInfo.origin = connInfo.address;
    connInfo.ssl_connection = connInfo.port;
    connInfo.ietf_version_or_minus_one = -1;
    connInfo.userdata = this;

    lws_client_connect_via_info(&connInfo);
    if (!wsi)
        throw new std::runtime_error("failed to connect");
}

void nativecord::Client::gatewayHandler(lws* wsi, char* in)
{
    nlohmann::json js = nlohmann::json::parse(in);

    if (js.contains("s") && !js["s"].is_null())
        js["s"].get_to(_lastSeq);

    gatewayCode code = js["op"];
    switch (code)
    {
        case GATEWAY_DISPATCH:
            {
                _emitter.fireEvent("dispatch", this, wsi, js);
                std::string eventName = js["t"].get<std::string>();
                if (_dispatchListeners.contains(eventName))
                    _dispatchListeners[eventName](this, wsi, js);
                break;
            }
        case GATEWAY_INVALID_SESSION:
            throw new std::runtime_error("invalid session");
            break;
        case GATEWAY_HELLO:
            identify(js);
            break;
    }
}

int nativecord::Client::websocketHandler(lws* wsi, lws_callback_reasons reason, char* in, size_t /*len*/)
{
    _wsi = wsi;

    switch (reason)
    {
        case LWS_CALLBACK_CLIENT_RECEIVE:
            this->gatewayHandler(wsi, in);
            break;
        case LWS_CALLBACK_TIMER:
            lws_callback_on_writable(wsi);
            break;
        case LWS_CALLBACK_CLIENT_WRITEABLE:
            {
                nlohmann::json heartbeat({{"op", GATEWAY_HEARTBEAT}});
                if (_lastSeq != -1)
                    heartbeat["d"] = _lastSeq;

                sendJson(heartbeat);
                lws_set_timer_usecs(wsi, _heartbeatInterval * 1000);
            }
            break;
        case LWS_CALLBACK_WS_PEER_INITIATED_CLOSE:
            uint16_t code;
            memcpy(&code, in, sizeof(uint16_t));
            code = ntohs(code);
            _emitter.fireEvent("disconnect", code);
            _wsi = nullptr;
            return -1;
    }
    _wsi = nullptr;
    return 0;
}

NC_EXPORT void nativecord::Client::pollEvents()
{
    return nativecord::net::pollEvents();
}

/*
     getters / setters
*/
NC_EXPORT inline void nativecord::Client::setToken(std::string token)
{
    _token = token;
}

NC_EXPORT inline std::string nativecord::Client::getToken() const
{
    return _token;
}

NC_EXPORT inline void nativecord::Client::setIntents(int intents)
{
    _intents = intents;
};
NC_EXPORT inline int nativecord::Client::getIntents() const
{
    return _intents;
};

NC_EXPORT inline const User* nativecord::Client::getUser() const
{
    return &_user;
}

NC_EXPORT void nativecord::Client::setPersona(userStatus status, std::vector<Activity>* activities)
{
    nlohmann::json js;
    js["op"] = GATEWAY_PRESENCE_UPDATE;
    js["d"] = {{"status", status}, {"afk", false}, {"activities", nlohmann::json::array({})}, {"since", nullptr}};

    if (activities)
    {
        if (activities->size() > 2)
            throw std::logic_error("can't have more than two concurrent activities");
        js["d"]["activities"] = *activities;
    }

    sendJson(js);
}
