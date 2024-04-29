#include "net.h"

#include <stdexcept>

#include <libwebsockets.h>

#include "nativecord/client.h"

int wssCallback(lws*, lws_callback_reasons, void*, void*, size_t);

static lws_protocols protocols[] = {{"wss", wssCallback, 0, NC_MAX_WSS_PACKETSIZE}, {0, 0}};
static lws_extension extensions[] = {{0, 0, 0}};

lws_context* ctx;
int connections;

int wssCallback(lws* wsi, lws_callback_reasons reason, void* user, void* in, size_t len)
{
    if (!user)
        return 0;
    auto client = reinterpret_cast<nativecord::Client*>(user);
    int ret = client->websocketHandler(wsi, reason, reinterpret_cast<char*>(in), len);
    switch (reason)
    {
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            connections++;
            break;
        case LWS_CALLBACK_SERVER_NEW_CLIENT_INSTANTIATED:
            connections--;
            if (!connections)
                nativecord::net::g_shouldStop = true;
            break;

    }
    return ret;
}

void nativecord::net::createContext()
{
    if (ctx != nullptr)
        throw new std::logic_error("attempt to create context when it already exists");

    lws_set_log_level(0, 0);

    lws_context_creation_info creationInfo{};
    creationInfo.protocols = protocols;
    creationInfo.extensions = extensions;
    creationInfo.gid = -1;
    creationInfo.uid = -1;
    creationInfo.user = 0;
    creationInfo.options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;

    ctx = lws_create_context(&creationInfo);
    if (!ctx)
        throw new std::runtime_error("failed to create context");
}

void nativecord::net::pollEvents()
{
    while (!g_shouldStop)
        lws_service(ctx, 0);
    lws_context_destroy(ctx);
}

lws_context* nativecord::net::getContext()
{
    if (ctx == nullptr)
        createContext();
    return ctx;
}
