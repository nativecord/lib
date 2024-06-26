#pragma once

#include <stddef.h>

#include <hv/WebSocketClient.h>

#include "events.h"

class WebsocketConnection : public hv::WebSocketClient
{
    public:
        WebsocketConnection() = delete;
        WebsocketConnection(const char* uri, bool reconnect = true, void* user = nullptr, hv::EventLoopPtr loop = NULL);
        ~WebsocketConnection();

        int connect();

        void* getUserData();
        void setShouldReconnect(bool shouldReconnect);

        template <typename Func> inline void on(std::string eventName, Func func) { _emitter.on(eventName, func); }

    private:
        char* _uri;
        void* _user;

        EventEmitter _emitter;
};