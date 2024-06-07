#pragma once

#include <string>
#include <unordered_map>

#include <hv/httpdef.h>
#include <nlohmann/json.hpp>

#include "nativecord/util/cache.h"
#include "nativecord/util/events.h"

#include "nativecord/discord/defs.h"
#include "nativecord/discord/enums/intents.h"
#include "nativecord/discord/objects/user.h"

#include "nativecord/util/websocket_connection.h"

namespace nativecord
{
    class Client;

    class Guild;
    class Channel;
    class Message;

    class ClientBase
    {
        protected:
            ClientBase(std::string token);
            ~ClientBase();

            void gatewayConnect();

        private:
            void registerEvents();

            void gatewayHeartbeat();
            void gatewayHello(nlohmann::json& js);
            void gatewaySend(nlohmann::json& js);

            void gatewayReceive(const std::string& data);

        public:
            std::pair<int, std::string> apiCall(const char* path, http_method method,
                                                nlohmann::json* payload = nullptr);

        protected:
            Cache<std::string, Guild>* _guildCache;
            Cache<std::string, Channel>* _channelCache;
            Cache<std::string, User>* _userCache;

            ClientIntents _intents = 0;
            std::string _token;
            User _user;
            EventEmitter _ev;

        private:
            std::unordered_map<std::string, void (*)(ClientBase* client, nlohmann::json& js)> _dispatchListeners;
            WebsocketConnection _ws;
            int _lastSequence;
    };
}
