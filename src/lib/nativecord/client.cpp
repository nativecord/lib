#include "client.h"
#include "endpoints.h"

#include "objects/message.h"
#include "util/assert.h"

#include <libwebsockets.h>

#include <nlohmann/json.hpp>

/*
    client implementation
*/
inline nativecord::Client::Client(std::string token) : _token(token)
{
    /*
        ready event
    */
    _emitter.registerEvent<Client*>("ready");
    _dispatchListeners["READY"] = [](Client* client, lws* /*wsi*/, void* jsPtr) {
        nlohmann::json* js = reinterpret_cast<nlohmann::json*>(jsPtr);
        js->at("d")["user"].get_to(client->_localUser);
        client->_emitter.fireEvent("ready", std::forward<Client*>(client));
    };

    /*
        message event
    */
    _emitter.registerEvent<Client*, Message*>("message");
    _dispatchListeners["MESSAGE_CREATE"] = [](Client* client, lws*, void* jsPtr) {
        nlohmann::json* js = reinterpret_cast<nlohmann::json*>(jsPtr);
        Message msg = js->at("d").get<Message>();
        client->_emitter.fireEvent("message", std::forward<Client*>(client), &msg);
    };

    /*
        disconnect event
    */
    _emitter.registerEvent<uint16_t>("disconnect");

    /*
        dispatch event
    */
    _emitter.registerEvent<Client*, lws*, nlohmann::json&>("dispatch");
}

NC_EXPORT bool nativecord::Client::connect()
{
    ASSERT(!_token.empty(), "invalid token (empty)");
    ASSERT(_intents != 0, "invalid intents (not set?)");

    if (!nativecord::websockets::g_context)
        nativecord::websockets::createContext();

    char* wss = reinterpret_cast<char*>(malloc(strlen(NC_WSS_URL) + 1));
    ASSERT(wss, "failed to allocate memory for wss uri");
    memcpy(wss, NC_WSS_URL, strlen(NC_WSS_URL));
    wss[strlen(NC_WSS_URL)] = '\0';

    lws_client_connect_info connInfo{};
    ASSERT(!lws_parse_uri(wss, &connInfo.protocol, &connInfo.address, &connInfo.port, &connInfo.path),
           "failed to parse wss uri");

    lws* wsi = nullptr;
    connInfo.pwsi = &wsi;
    connInfo.context = nativecord::websockets::g_context;
    connInfo.host = connInfo.address;
    connInfo.origin = connInfo.address;
    connInfo.ssl_connection = connInfo.port;
    connInfo.ietf_version_or_minus_one = -1;
    connInfo.userdata = this;

    lws_client_connect_via_info(&connInfo);
    ASSERT(wsi, "client connect failed");

    nativecord::websockets::g_connections++;

    return true;
}

int nativecord::Client::handleWss(lws* wsi, lws_callback_reasons reason, char* in, size_t /*len*/)
{
    switch (reason)
    {
        case LWS_CALLBACK_CLIENT_RECEIVE:
            _wsInterface = wsi;
            this->handleGateway(wsi, in);
            _wsInterface = nullptr;
            break;
        case LWS_CALLBACK_TIMER:
            lws_callback_on_writable(wsi);
            break;
        case LWS_CALLBACK_CLIENT_WRITEABLE:
            {
                nlohmann::json heartbeat;
                heartbeat["op"] = GATEWAY_HEARTBEAT;
                if (_lastSeq != -1)
                    heartbeat["d"] = _lastSeq;
                else
                    heartbeat["d"] = nullptr;

                sendJSON(wsi, &heartbeat);
                lws_set_timer_usecs(wsi, _heartbeatInterval * 1000);
                break;
            }
        case LWS_CALLBACK_WS_PEER_INITIATED_CLOSE:
            {
                uint16_t code;
                memcpy(&code, in, sizeof(uint16_t));
                code = ntohs(code);
                _emitter.fireEvent("disconnect", std::forward<uint16_t>(code));

                nativecord::websockets::g_connections--;
                if (nativecord::websockets::g_connections < 1)
                    nativecord::websockets::g_shouldStop = true;
                return -1;
            }
    }
    return 0;
}

void nativecord::Client::handleGateway(lws* wsi, char* in)
{
    nlohmann::json payload = nlohmann::json::parse(in);
    if (payload.contains("s") && !payload.at("s").is_null())
        payload.at("s").get_to<int>(_lastSeq);

    ASSERT(payload.contains("op"), "received gateway payload without operation?");

    gatewayCode code = payload.at("op").get<gatewayCode>();

    switch (code)
    {
        case GATEWAY_DISPATCH:
            {
                _emitter.fireEvent("dispatch", this, wsi, payload);
                std::string eventName = payload.at("t").get<std::string>();
                if (_dispatchListeners.contains(eventName))
                    _dispatchListeners[eventName](this, wsi, &payload);
                break;
            }
        case GATEWAY_INVALID_SESSION:
            PANIC("received GATEWAY_INVALID_SESSION");
            break;
        case GATEWAY_HELLO:
            {
                _heartbeatInterval = payload.at("d").at("heartbeat_interval").get<int>();
                lws_set_timer_usecs(wsi, _heartbeatInterval * 1000);

                nlohmann::json identifyPayload;
                identifyPayload["op"] = GATEWAY_IDENTIFY;

                nlohmann::json data;
                data["token"] = _token;
                data["intents"] = _intents;
                data["compress"] = false;
                data["large_threshold"] = 250;

                data["properties"]["$os"] = "Windows";
                data["properties"]["$browser"] = "Chrome";

                identifyPayload["d"] = data;
                sendJSON(wsi, &identifyPayload);
                break;
            }
    }
}

void nativecord::Client::sendJSON(lws* wsi, void* jsPtr) const
{
    nlohmann::json& js = *reinterpret_cast<nlohmann::json*>(jsPtr);

    std::string payloadStr = js.dump();
    unsigned char* buff = reinterpret_cast<unsigned char*>(malloc(LWS_PRE + payloadStr.size()));
    ASSERT(buff, "failed to allocate buffer for json str");
    memcpy(buff + LWS_PRE, payloadStr.c_str(), payloadStr.size());

    ASSERT(lws_write(wsi, buff + LWS_PRE, payloadStr.size(), LWS_WRITE_TEXT) >= 0, "lws_write failed");

    free(buff);
}

int nativecord::Client::wssCallback(lws* wsi, lws_callback_reasons reason, void* user, void* in, size_t len)
{
    if (!user)
        return 0;

    auto client = reinterpret_cast<nativecord::Client*>(user);
    return client->handleWss(wsi, reason, reinterpret_cast<char*>(in), len);
}

/*
    client setters / getters
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
    return &_localUser;
}

// TO-DO: add activities
NC_EXPORT void nativecord::Client::setPersona(userStatus status, std::vector<Activity>* activities)
{
    ASSERT(_wsInterface, "called setPersona from outside an event");
    nlohmann::json payload;
    payload["op"] = GATEWAY_PRESENCE_UPDATE;
    payload["d"] = {{"status", status}, {"afk", false}, {"activities", nlohmann::json::array({})}, {"since", nullptr}};

    if (activities)
    {
        ASSERT(activities->size() < 2, "can't have more than two activities at once");
        payload["d"]["activities"] = *activities;
    }

    sendJSON(_wsInterface, &payload);
}

/*
    base connection implementation
*/
static lws_protocols protocols[] = {{"wss", nativecord::Client::wssCallback, 0, NC_MAX_WSS_PACKETSIZE}, {0, 0}};
static lws_extension extensions[] = {{0, 0, 0}};

void nativecord::websockets::createContext()
{
    ASSERT(g_context == nullptr, "attempted to create context while g_context != nullptr");

    lws_set_log_level(0, 0);

    lws_context_creation_info creationInfo{};
    creationInfo.protocols = protocols;
    creationInfo.extensions = extensions;
    creationInfo.gid = -1;
    creationInfo.uid = -1;
    creationInfo.user = 0;
    creationInfo.options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;

    g_context = lws_create_context(&creationInfo);
    ASSERT(g_context, "failed to create libwebosckets context");
}

void nativecord::websockets::pollEvents()
{
    while (!g_shouldStop)
        lws_service(g_context, 0);
    lws_context_destroy(g_context);
}