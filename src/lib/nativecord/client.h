#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "nativecord/classes/cache.h"

#include "nativecord/classes/events.h"
#include "nativecord/util/macros.h"

#include "nativecord/discord/enums/intents.h"
#include "nativecord/discord/objects/user.h"

class WebsocketConnection;

struct lws;
struct lws_context;
NC_WARNING_PUSH
NC_DISABLE_WARNING(4471)
enum lws_callback_reasons;
NC_WARNING_POP

class Guild;
class User;

/*
    TO-DO:
        dynamically figure these out
*/
#define NC_DEFAULT_GUILD_CACHE_SIZE 250
#define NC_DEFAULT_USER_CACHE_SIZE 250

namespace nativecord
{
    int _clientWsCallback(lws* wsi, lws_callback_reasons reason, void* user, void* in, size_t len);
    class Client
    {
        public:
            Client(std::string token = "");
            ~Client();

            inline void setToken(std::string token) { _token = token; }
            inline std::string getToken() const { return _token; }

            inline void setIntents(clientIntents val) { _intents = val; };
            inline clientIntents getIntents() const { return _intents; };

            inline const User* getUser() const { return &_user; };

            // void setPersona(userStatus status, std::vector<Activity>* activities = {});

            void connect();

            template <typename Func> inline void on(std::string eventName, Func func) { _emitter.on(eventName, func); }

            int __wsReceive(lws* wsi, lws_callback_reasons reason, char* in, size_t len);

            Cache<snowflake, Guild>* getGuildCache() const { return _guildCache; }
            Cache<snowflake, User>* getUserCache() const { return _userCache; }

            std::unique_ptr<char[]> apiCall(const char* path, const char* method, nlohmann::json* payload = nullptr);

        private:
            friend int _clientWsCallback(lws* wsi, lws_callback_reasons reason, void* user, void* in, size_t len);

            void onGateway(lws* wsi, nlohmann::json& js);

            void wsSendJson(nlohmann::json& js);

            void identify(nlohmann::json& js);
            void registerEvents();

            std::unordered_map<std::string, void (*)(Client* client, lws* wsi, nlohmann::json& js)> _dispatchListeners;

            EventEmitter _emitter;

            std::string _token;
            clientIntents _intents;

            int _lastSequence;
            int _heartbeatInterval;

            WebsocketConnection* _ws;

            Cache<snowflake, Guild>* _guildCache;
            Cache<snowflake, User>* _userCache;

            User _user;
    };
}
