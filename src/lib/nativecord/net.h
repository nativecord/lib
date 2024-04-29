#pragma once

/*
    TO-DO:
        add ring buffer?
        figure out max sizes for packets (and ring buffer once implemented)
*/
#define NC_MAX_WSS_PACKETSIZE 4096 * 10

struct lws_context;
namespace nativecord
{
    namespace net
    {
        void createContext();
        void pollEvents();
        lws_context* getContext();

        static bool g_shouldStop = false;
    }
}