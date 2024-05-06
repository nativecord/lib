#include <nativecord/client.h>

#include <nativecord/discord/objects/channel.h>
#include <nativecord/discord/objects/message.h>

#include <nlohmann/json.hpp>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <stdexcept>

#include "util/log.h"

#define ASSERT(cond, msg)                                                                                              \
    if (!(cond))                                                                                                       \
    {                                                                                                                  \
        Log::error(msg);                                                                                               \
        return -1;                                                                                                     \
    }

int main(int /*argc*/, char* argv[])
{
#ifdef _WIN32
    Log::setupConsole();
#endif

    Log::info("Loading config");
    std::filesystem::path binPath = std::filesystem::absolute(std::filesystem::path(argv[0])).parent_path();
    std::filesystem::path configPath(binPath.string());
    configPath /= "config.json";
    ASSERT(std::filesystem::exists(configPath), "config.json does not exist");

    std::ifstream configStream(configPath);
    ASSERT(configStream.is_open(), "failed to open config.json");

    nlohmann::json config = nlohmann::json::parse(configStream);

    std::string token;
    ASSERT(config.contains("token"), "invalid config (missing token)");
    config["token"].get_to(token);

    nativecord::Client* client = new nativecord::Client();
    client->setToken(token);
    client->setIntents(INTENT_GUILDS | INTENT_GUILD_MESSAGES | INTENT_GUILD_MEMBERS);

    client->on("ready", [](nativecord::Client* client) {
        Log::info("client is ready");
        Log::info("Logged in as {} ({})", client->getUser()->username, client->getUser()->id);
    });

    client->on("dispatch", [](nativecord::Client* client, nlohmann::json& js) {
        std::string type = js["t"].get<std::string>();
        Log::info("received dispatch of type: {}", type);
        if (type == "MESSAGE_CREATE")
        {
            Message msg = js["d"];
            if (msg.author.value().id == client->getUser()->id)
                return;

            if (msg.content == "hi")
            {
                snowflake channelId = js["d"]["channel_id"].get<snowflake>();
                auto TestChannel = new Channel(client, channelId);

                Message reply{};
                reply.content = "hello";
                reply.message_reference =
                    std::make_optional<MessageReference>({msg.id, std::nullopt, std::nullopt, std::nullopt});

                TestChannel->sendMessage(&reply);
            }
        }
    });

    client->on("disconnect",
               [](nativecord::Client*, int reason) { Log::info("Client disconnected with reason: {}", reason); });

    client->connect();

    std::string in;
    while (std::getline(std::cin, in))
    {
        if (in == "stop")
        {
            break;
        }
    }

    return 0;
}
