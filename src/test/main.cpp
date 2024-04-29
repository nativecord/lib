#include <nativecord/client.h>

#include <nativecord/util/assert.h>

#include <nativecord/objects/message.h>

#include <nlohmann/json.hpp>

#include "util/log.h"

#include <filesystem>
#include <fstream>

#include <chrono>

int main(int /*argc*/, char* argv[])
{
#ifdef _WIN32
    Log::SetupConsole();
#endif

    Log::Info("Parsing config file");
    std::filesystem::path binPath = std::filesystem::absolute(std::filesystem::path(argv[0])).parent_path();
    std::string configPath(binPath.string() + "\\config.json");

    ASSERT(std::filesystem::exists(configPath), "config.json does not exist.");
    std::ifstream config(configPath);
    ASSERT(config.is_open(), "unable to open config.json");
    nlohmann::json configjs = nlohmann::json::parse(config);

    ASSERT(configjs.contains("token"), "invalid config (missing token)");

    std::string token = configjs["token"].get<std::string>();

    nativecord::Client client;
    client.setToken(token);
    client.setIntents(INTENT_GUILDS | INTENT_GUILD_MESSAGES | INTENT_GUILD_MEMBERS);
    ASSERT(client.connect(), "client failed to connect");

    client.on("ready", [](nativecord::Client* client) {
        Log::Info("Client is ready");
        Log::Info("Logged in as {} | {}", client->getUser()->username, client->getUser()->id);

        auto now = std::chrono::system_clock::now().time_since_epoch();

        int currTime = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(now).count());

        Activity testActivity;
        testActivity.type = ACTIVITY_WATCHING;
        testActivity.name = "something";
        testActivity.timestamps = {currTime, {}};
        std::vector<Activity> activities;
        activities.push_back(testActivity);

        client->setPersona(STATUS_IDLE, &activities);
    });

    client.on("dispatch", [](nativecord::Client* client, lws*, nlohmann::json& js) {
        std::string type = js["t"].get<std::string>();
        Log::Verbose("{} received dispatch of type: {}", client->getUser()->username, type);
    });

    client.on("message", [](nativecord::Client* /*client*/, Message* msg) {
        Log::Verbose("Client received message from {} : {}", msg->author.global_name, msg->content);
    });

    client.on("disconnect", [](uint16_t code) { Log::Info("Client disconnected with code: {}", code); });

    nativecord::websockets::pollEvents();

    return 0;
}