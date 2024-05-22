#pragma once

#include "util/macros.h"

#include "internal/clientbase.h"

namespace nativecord
{
    class Client : private ClientBase
    {
        public:
            NC_EXPORT Client();
            NC_EXPORT Client(std::string token);

            NC_EXPORT ~Client();

            NC_EXPORT void connect();

            NC_EXPORT inline const User* getUser() const;

            NC_EXPORT inline void setToken(std::string token);
            NC_EXPORT inline std::string getToken();

            NC_EXPORT inline void setIntents(ClientIntents intents);
            NC_EXPORT inline ClientIntents getIntents();

            NC_EXPORT std::shared_ptr<Channel> getChannel(std::string id);
            NC_EXPORT std::shared_ptr<Guild> getGuild(std::string id);

            /*
                Available events (name - args)

                ready - Client*
                message - Client*, Message* msg
                disconnect - Client*
                dispatch - Client*, std::string name, nlohmann::json& data
                error - Client*, std::string msg

                TO-DO:
                guild_create - Client*, std::shared_pointer<Guild> guild
                guild_delete - Client*, snowflake guildId
            */
            template <typename Func> inline void on(std::string eventName, Func func) { _ev.on(eventName, func); }
    };
}
