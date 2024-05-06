#pragma once

#include <stddef.h>

#include <hv/WebSocketClient.h>

#include "nativecord/classes/events.h"

/*
    TO-DO:
        calculate a logical value for this
*/
#define NC_MAX_WSS_PACKETSIZE 4096

class WebsocketConnection : public hv::WebSocketClient
{
    public:
        WebsocketConnection() = delete;
        WebsocketConnection(const char* uri, void* user = nullptr, hv::EventLoopPtr loop = NULL);
        ~WebsocketConnection();

        int connect();
        void* getUserData();

        template <typename Func> inline void on(std::string eventName, Func func) { _emitter.on(eventName, func); }
    private:
        char* _uri;
        void* _user;
        
        EventEmitter _emitter;
};