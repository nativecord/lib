#include "websocket_connection.h"

#include <libwebsockets.h>

#include <stdexcept>
#include <vector>

std::vector<char> fragmentBuffer;

int wsCallback(lws* wsi, int reason, void* user, void* in, size_t len)
{
    const lws_protocols* prot = lws_get_protocol(wsi);

    if (!prot)
        return 0;
    WebsocketConnection* conn = reinterpret_cast<WebsocketConnection*>(prot->user);

    int ret = 0;
    if (reason == LWS_CALLBACK_CLIENT_RECEIVE)
    {
        char* inChar = reinterpret_cast<char*>(in);
        bool isFinal = lws_is_final_fragment(wsi);
        if (!isFinal)
        {
            fragmentBuffer.insert(fragmentBuffer.end(), inChar, inChar + len);
        }
        else
        {
            if (!fragmentBuffer.empty())
            {
                fragmentBuffer.insert(fragmentBuffer.end(), inChar, inChar + len);
                fragmentBuffer.push_back('\0');
                ret = conn->invokeCallback(wsi, reason, user, fragmentBuffer.data(), fragmentBuffer.size());
                fragmentBuffer.clear();
            }
            else
            {
                ret = conn->invokeCallback(wsi, reason, user, in, len);
            }
        }
    }
    else
    {
        ret = conn->invokeCallback(wsi, reason, user, in, len);
    }

    switch (reason)
    {
        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
        case LWS_CALLBACK_WS_PEER_INITIATED_CLOSE:
            conn->_shouldStop = true;
            break;
        default:
            break;
    }

    return ret;
}

WebsocketConnection::WebsocketConnection(const char* uri, WebsocketCallbackFn cb, void* user) : _callback(cb)
{
    size_t uriLength = strlen(uri);
    char* uriCopy = reinterpret_cast<char*>(malloc(uriLength + 1));
    if (!uriCopy)
        throw new std::runtime_error("failed to allocate memory for uri");
    memcpy(uriCopy, uri, uriLength);
    uriCopy[uriLength] = '\0';

    lws_client_connect_info connInfo{};
    if (lws_parse_uri(uriCopy, &connInfo.protocol, &connInfo.address, &connInfo.port, &connInfo.path))
    {
        free(uriCopy);
        throw new std::exception("failed to parse uri");
    }
    connInfo.host = connInfo.address;
    connInfo.origin = connInfo.address;
    connInfo.ssl_connection = LCCSCF_USE_SSL | LCCSCF_PIPELINE;

    connInfo.context = getContext();
    connInfo.ietf_version_or_minus_one = -1;

    connInfo.userdata = user;

    connInfo.pwsi = &_wsi;

    _wsi = lws_client_connect_via_info(&connInfo);

    free(uriCopy);

    if (!_wsi)
        throw new std::runtime_error("websocket failed to connect");

    _wsi = nullptr;
}

WebsocketConnection::~WebsocketConnection()
{
    _shouldStop = true;
}

void WebsocketConnection::pollEvents()
{
    while (!_shouldStop)
        lws_service(getContext(), 0);
    lws_context_destroy(getContext());
}

int WebsocketConnection::invokeCallback(lws* wsi, int reason, void* user, void* in, size_t len)
{
    _wsi = wsi;
    int ret = _callback(wsi, reason, user, in, len);
    _wsi = nullptr;
    return ret;
}

lws* WebsocketConnection::getInterface()
{
    if (!_wsi)
        throw new std::runtime_error("called getInterface outside of callback");
    return _wsi;
}

lws_context* WebsocketConnection::createContext()
{
    lws_set_log_level(0, 0);

    lws_context_creation_info cci{};

    lws_protocols prot[] = {{"ws", reinterpret_cast<lws_callback_function*>(wsCallback), 0, NC_MAX_WSS_PACKETSIZE, 0, this, 0}, {0, 0, 0, 0, 0, 0}};
    static lws_extension ext[] = {{0, 0, 0}};

    cci.protocols = prot;
    cci.extensions = ext;
    cci.gid = -1;
    cci.uid = -1;
    cci.user = 0;
    cci.options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;

    _context = lws_create_context(&cci);
    if (!_context)
        throw new std::runtime_error("failed to create context");
    return _context;
}

lws_context* WebsocketConnection::getContext()
{
    static lws_context* context = nullptr;
    if (!context)
        context = createContext();
    return context;
}
