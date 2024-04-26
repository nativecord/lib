#pragma once

#include "gateway.h"
#include "util/macros.h"

#include <string>

/*
    TO-DO:
        figure out max possible size sent by discord
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
            NC_EXPORT inline Client(std::string token = "");

            NC_EXPORT inline void setToken(std::string token)
            {
                _token = token;
            }
            NC_EXPORT inline std::string getToken() const
            {
                return _token;
            };

            NC_EXPORT inline void setIntents(int intents)
            {
                _intents = intents;
            };
            NC_EXPORT inline int getIntents() const
            {
                return _intents;
            };

            NC_EXPORT bool connect();

            int handleWss(lws* wsi, lws_callback_reasons reason, char* in, size_t len);
            static int wssCallback(lws* wsi, lws_callback_reasons reason, void* user, void* in, size_t len);

        private:
            void handleGateway(lws* wsi, char* in);
            void handleDispatch(lws* wsi, std::string eventName, void* dataPtr);
            void sendJSON(lws* wsi, void* jsPtr) const;

            std::unordered_map<std::string, void (*)(Client* client, lws* wsi, void* jsPtr)> _dispatchListeners;

            std::string _token;

            int _heartbeatInterval = -1;
            int _lastSeq = -1;
            int _intents = -1;
    };
}