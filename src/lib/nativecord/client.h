#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "nativecord/classes/cache.h"
#include "nativecord/classes/events.h"
#include "nativecord/classes/websocket_connection.h"

#include "nativecord/util/macros.h"

#include "nativecord/discord/enums/intents.h"
#include "nativecord/discord/objects/guild.h"
#include "nativecord/discord/objects/user.h"

#include <hv/httpdef.h>

/*
    TO-DO:
        dynamically figure these out
*/
#define NC_DEFAULT_GUILD_CACHE_SIZE 250
#define NC_DEFAULT_USER_CACHE_SIZE 250

namespace nativecord
{
    class Client
    {
        public:
            NC_EXPORT Client(std::string token = "");
            NC_EXPORT ~Client();

            inline void setToken(std::string token) { _token = token; }
            inline std::string getToken() const { return _token; }

            inline void setIntents(clientIntents val) { _intents = val; };
            inline clientIntents getIntents() const { return _intents; };

            inline const User* getUser() const { return &_user; };

            // void setPersona(userStatus status, std::vector<Activity>* activities = {});

            NC_EXPORT int connect();

            template <typename Func> inline void on(std::string eventName, Func func) { _emitter.on(eventName, func); }

            Cache<snowflake, Guild>* getGuildCache() const { return _guildCache; }
            Cache<snowflake, User>* getUserCache() const { return _userCache; }

            std::pair<int, std::string> apiCall(const char* path, http_method method,
                                                nlohmann::json* payload = nullptr);

            void __processReceive(const std::string& msg);

            inline bool isConnected() const { return _connected; };
        private:
            void sendHeartbeat();
            void sendJson(nlohmann::json& js);

            void identify(nlohmann::json& js);
            void registerEvents();

            std::unordered_map<std::string, void (*)(Client* client, nlohmann::json& js)> _dispatchListeners;

            EventEmitter _emitter;

            std::string _token;
            clientIntents _intents;

            int _lastSequence;
            bool _connected = false;

            WebsocketConnection* _ws;

            Cache<snowflake, Guild>* _guildCache;
            Cache<snowflake, User>* _userCache;

            User _user;
    };
}
