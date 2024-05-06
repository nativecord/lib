#include "websocket_connection.h"

WebsocketConnection::WebsocketConnection(const char* uri, void* user, hv::EventLoopPtr loop)
    : WebSocketClient(loop), _user(user)
{
    _uri = new char[strlen(uri)];
    strcpy(_uri, uri);

    _emitter.registerEvent<WebsocketConnection*, std::string>("open");
    _emitter.registerEvent<WebsocketConnection*, const std::string&>("message");
    _emitter.registerEvent<WebsocketConnection*, int>("close");
}

WebsocketConnection::~WebsocketConnection() {}

int WebsocketConnection::connect()
{
    onopen = [this]() { _emitter.fireEvent("open", this, this->getHttpResponse()->Dump()); };

    onmessage = [this](const std::string& msg) { _emitter.fireEvent("message", this, msg); };

    onclose = [this]() { _emitter.fireEvent("close", this, channel->error()); };

    setPingInterval(10000);

    reconn_setting_t reconnect;
    reconn_setting_init(&reconnect);
    reconnect.min_delay = 1000;
    reconnect.max_delay = 10000;
    reconnect.delay_policy = 2;
    setReconnect(&reconnect);

    return open(_uri);
}

void* WebsocketConnection::getUserData() { return _user; }
