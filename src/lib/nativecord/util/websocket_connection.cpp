#include "websocket_connection.h"

WebsocketConnection::WebsocketConnection(const char* uri, bool reconnect, void* user, hv::EventLoopPtr loop)
    : WebSocketClient(loop), _user(user)
{
    _uri = new char[strlen(uri)];
    strcpy(_uri, uri);

    _emitter.registerEvent<WebsocketConnection*, std::string>("open");
    _emitter.registerEvent<WebsocketConnection*, const std::string&>("message");
    _emitter.registerEvent<WebsocketConnection*>("close");

    setShouldReconnect(reconnect);
    setPingInterval(10000);

    onopen = [this]() { _emitter.fireEvent("open", this, this->getHttpResponse()->Dump()); };
    onmessage = [this](const std::string& msg) { _emitter.fireEvent("message", this, msg); };
    onclose = [this]() { _emitter.fireEvent("close", this); };
}

WebsocketConnection::~WebsocketConnection() {}

int WebsocketConnection::connect() { return open(_uri); }

void* WebsocketConnection::getUserData() { return _user; }

void WebsocketConnection::setShouldReconnect(bool shouldReconnect)
{
    if (shouldReconnect)
    {
        reconn_setting_t reconnect;
        reconn_setting_init(&reconnect);
        reconnect.min_delay = 100;
        reconnect.max_delay = 5000;
        reconnect.delay_policy = 2;
        setReconnect(&reconnect);
        return;
    }
    setReconnect(nullptr);
}
