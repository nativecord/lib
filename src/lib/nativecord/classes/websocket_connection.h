#pragma once

#include <stddef.h>


/*
    TO-DO:
        calculate a logical value for this
*/
#define NC_MAX_WSS_PACKETSIZE 4096

struct lws;
struct lws_context;

typedef int (*WebsocketCallbackFn)(lws*, int, void*, void*, size_t);

class WebsocketConnection
{
    public:
        WebsocketConnection() = delete;
        WebsocketConnection(const char* uri, WebsocketCallbackFn cb, void* user = nullptr);
        ~WebsocketConnection();

        void pollEvents();
        int invokeCallback(lws* wsi, int reason, void* user, void* in, size_t len);

        lws* getInterface();

    private:
        friend int wsCallback(lws*, int, void*, void*, size_t);
        int (*_callback)(lws*, int, void*, void*, size_t);

        lws_context* createContext();
        lws_context* getContext();

        bool _shouldStop = false;

        lws* _wsi;
        lws_context* _context;
};