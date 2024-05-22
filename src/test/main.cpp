#include <nativecord/client.h>

#include <nativecord/discord/objects/channel.h>
#include <nativecord/discord/objects/message.h>

#include <nlohmann/json.hpp>

#include <chrono>
#include <csignal>
#include <filesystem>
#include <fstream>
#include <thread>

#include "util/log.h"

#include "commandmanager.h"
#include "commands/general.h"

using namespace nativecord;

/*
    error handling helpers
*/
template <typename... Args> inline void displayError(const std::format_string<Args...> fmt, Args&&... args)
{
    std::string msg(std::vformat(fmt.get(), std::make_format_args(args...)));
    Log::error("{}", msg);
    Log::error("Press any key to exit...");
    std::cin;
    exit(-1);
}

#define ASSERT(cond, msg)                                                                                              \
    if (!(cond))                                                                                                       \
        displayError(msg);

// ctrl+c handling stuff
bool running = true;
void sigintHandler(int signum) { running = false; }

int main(int, char* argv[])
{
    // register sigint listener
    std::signal(SIGINT, sigintHandler);

#ifdef _WIN32
    Log::setupConsole(); // enable ANSI on Windows
#endif

    /*
        config parsing
    */
    std::filesystem::path path = std::filesystem::absolute(std::filesystem::path(argv[0])).parent_path();
    path /= "config.json";

    ASSERT(std::filesystem::exists(path), "missing config.json");

    std::ifstream configStream(path);
    ASSERT(configStream.is_open(), "failed to open config.json");

    nlohmann::json config;
    try
    {
        config = nlohmann::json::parse(configStream);
        ASSERT(config.contains("token"), "missing token on config.json");
    }
    catch (std::exception ex)
    {
        displayError("failed to parse config.json ({})", ex.what());
    }

    /*
        set command manager prefix
    */
    std::string prefix(".t ");
    if (config.contains("prefix"))
        config["prefix"].get_to(prefix);
    CommandManager::get()->setPrefix(prefix);

    /*
        instantiate client
    */
    Client* client = new Client();
    client->setToken(config["token"]);
    client->setIntents(INTENT_GUILDS | INTENT_GUILD_MESSAGES | INTENT_GUILD_MEMBERS);

    /*
        event listeners
    */
    client->on("dispatch", [](Client* client, std::string name, nlohmann::json&) {
        Log::verbose("received dispatch of type: {}", name);
    });
    client->on("disconnect", [](Client*) { Log::info("client disconnected"); });
    client->on("message",
               [](Client* client, nativecord::Message* msg) { CommandManager::get()->handleMessage(client, msg); });

    /*
        connect and await exit
    */
    client->connect();

    Log::info("Press CTRL + C to exit");
    while (running)
        std::this_thread::sleep_for(std::chrono::milliseconds(250));

    return 0;
}
