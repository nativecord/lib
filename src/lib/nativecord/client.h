#pragma once

#include "gateway.h"

#include "util/events.h"
#include "util/macros.h"

#include "objects/user.h"
#include "objects/activity.h"

#include <string>
#include <unordered_map>

/*
    TO-DO:
        add ring buffer?
        add error handling (replace asserts)
        figure out realistic sizes for packets (and ring buffer once implemented)
*/

#define NC_MAX_WSS_PACKETSIZE 4096 * 10

struct lws;
struct lws_context;
enum lws_callback_reasons : int;

namespace nativecord
{
    namespace websockets
    {
        void createContext();
        void pollEvents();

        static lws_context* g_context = nullptr;
        static int g_connections = 0;
        static bool g_shouldStop = false;
    }

    class Client
    {
        public:
            NC_EXPORT Client(std::string token = "");

            NC_EXPORT inline void setToken(std::string token);
            NC_EXPORT inline std::string getToken() const;

            NC_EXPORT inline void setIntents(int intents);
            NC_EXPORT inline int getIntents() const;

            NC_EXPORT inline const User* getUser() const;

            NC_EXPORT void setPersona(userStatus status, Activity activities[2] = nullptr);

            NC_EXPORT bool connect();

            int handleWss(lws* wsi, lws_callback_reasons reason, char* in, size_t len);
            static int wssCallback(lws* wsi, lws_callback_reasons reason, void* user, void* in, size_t len);

            EventEmitter _emitter;

        private:
            lws* _wsInterface = nullptr;

            void handleGateway(lws* wsi, char* in);
            void sendJSON(lws* wsi, void* jsPtr) const;

            std::unordered_map<std::string, void (*)(Client* client, lws* wsi, void* jsPtr)> _dispatchListeners;

            std::string _token;

            int _heartbeatInterval = -1;
            int _lastSeq = -1;
            int _intents = -1;

            User _localUser;
    };
}