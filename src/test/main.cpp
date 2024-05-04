#include <nativecord/client.h>

#include <nativecord/discord/objects/channel.h>

#include <nlohmann/json.hpp>

#include <filesystem>
#include <fstream>
#include <stdexcept>

#include "util/log.h"

int main(int /*argc*/, char* argv[])
{
#ifdef _WIN32
    Log::setupConsole();
#endif

    Log::info("Loading config");
    std::filesystem::path binPath = std::filesystem::absolute(std::filesystem::path(argv[0])).parent_path();
    std::string configPath(binPath.string() + "\\config.json");
    if (!std::filesystem::exists(configPath))
        throw new std::runtime_error("config.json does not exist");
    std::ifstream configStream(configPath);
    if (!configStream.is_open())
        throw new std::runtime_error("failed to open config.json");

    nlohmann::json config = nlohmann::json::parse(configStream);

    std::string token;
    if (!config.contains("token"))
        throw new std::runtime_error("config does not have token");
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
            std::string content = js["d"]["content"];
            Log::info("content: {}", content);

            if (content == "ping")
            {
                snowflake channelId = js["d"]["channel_id"].get<snowflake>();
                auto TestChannel = new Channel(client, channelId);
                TestChannel->sendMessage("pong !!");
            }
        }
    });

    client->on("disconnect", [](uint16_t code) { Log::info("Client disconnected with code: {}", code); });

    client->connect();
}
