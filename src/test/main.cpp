#include <nativecord/client.h>
#include <nativecord/util/assert.h>

#include <nlohmann/json.hpp>

#include "util/log.h"

#include <fstream>
#include <filesystem>

int main(int /*argc*/, char* argv[])
{
#ifdef _WIN32
    Log::SetupConsole();
#endif

    Log::Info("Parsing config file");
    std::filesystem::path binPath = std::filesystem::absolute(std::filesystem::path(argv[0])).parent_path();
    std::string configPath(binPath.string() +  "\\config.json");

    ASSERT(std::filesystem::exists(configPath), "config.json does not exist.");
    std::ifstream config(configPath);
    ASSERT(config.is_open(), "unable to open config.json");
    nlohmann::json configjs = nlohmann::json::parse(config);

    ASSERT(configjs.contains("token"), "invalid config (missing token)");

    std::string token = configjs["token"].get<std::string>();

    auto client = new nativecord::Client();
    client->setToken(token);
    client->setIntents(INTENT_GUILDS | INTENT_GUILD_MESSAGES | INTENT_GUILD_MEMBERS);
    ASSERT(client->connect(), "client failed to connect");

    nativecord::websockets::pollEvents();

    return 0;
}