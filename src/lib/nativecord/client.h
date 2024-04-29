#pragma once

#include "gateway.h"

#include "util/events.h"
#include "util/macros.h"

#include "objects/activity.h"
#include "objects/user.h"

#include <string>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

struct lws;

NC_WARNING_PUSH
NC_DISABLE_WARNING(4471)
enum lws_callback_reasons;
NC_WARNING_POP

namespace nativecord
{
    class Client
    {
        public:
            NC_EXPORT Client(std::string token = "");

            NC_EXPORT inline void setToken(std::string token);
            NC_EXPORT inline std::string getToken() const;

            NC_EXPORT inline void setIntents(int intents);
            NC_EXPORT inline int getIntents() const;

            NC_EXPORT inline const User* getUser() const;

            NC_EXPORT void setPersona(userStatus status, std::vector<Activity>* activities = {});

            NC_EXPORT void connect();

            void gatewayHandler(lws* wsi, char* in);
            int websocketHandler(lws* wsi, lws_callback_reasons reason, char* in, size_t len);

            template <typename Func> inline void NC_EXPORT on(std::string eventName, Func func)
            {
                _emitter.on(eventName, func);
            }

            NC_EXPORT static void pollEvents();

        private:
            void identify(nlohmann::json& js);
            void registerEvents();

            void sendJson(nlohmann::json& js);

            std::unordered_map<std::string, void (*)(Client* client, lws* wsi, nlohmann::json& js)> _dispatchListeners;
            EventEmitter _emitter;

            std::string _token;
            int _intents = -1;

            int _lastSeq = -1;
            int _heartbeatInterval = -1;

            User _user;

            lws* _wsi;
    };
}