#pragma once

#include "nativecord/util/macros.h"

/*
    TO-DO:
        calculate a logical value for this
*/
#define NC_MAX_WSS_PACKETSIZE 4096

struct lws;
struct lws_context;
NC_WARNING_PUSH
NC_DISABLE_WARNING(4471)
enum lws_callback_reasons;
NC_WARNING_POP

typedef int (*WebsocketCallbackFn)(lws*, lws_callback_reasons, void*, void*, size_t);

class WebsocketConnection
{
    public:
        WebsocketConnection() = delete;
        WebsocketConnection(const char* uri, WebsocketCallbackFn cb, void* user = nullptr);
        ~WebsocketConnection();

        void pollEvents();
        int invokeCallback(lws* wsi, lws_callback_reasons reason, void* user, void* in, size_t len);

        lws* getInterface();

    private:
        friend int wsCallback(lws*, lws_callback_reasons, void*, void*, size_t);
        int (*_callback)(lws*, lws_callback_reasons, void*, void*, size_t);

        lws_context* createContext();
        lws_context* getContext();

        bool _shouldStop = false;

        lws* _wsi;
        lws_context* _context;
};