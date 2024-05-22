#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "nativecord/client.h"

#include "nativecord/discord/objects/channel.h"
#include "nativecord/discord/objects/message.h"

using namespace nativecord;

typedef std::function<void(Client* client, std::shared_ptr<Channel> channel, nativecord::Message* msg,
                           std::vector<std::string>* args)>
    MessageHandler;

class Command
{
    public:
        Command(std::string name, std::string desc, MessageHandler handler, bool requireOwner = false)
            : _name(name), _desc(desc), _handler(handler), _requireOwner(requireOwner){};

        inline std::string getName() { return _name; }

        void run(Client* client, std::shared_ptr<Channel> channel, nativecord::Message* msg,
                 std::vector<std::string>* args)
        {
            _handler(client, channel, msg, args);
        }

        bool requiresOwner() { return _requireOwner; }

    private:
        Command() = delete;
        ~Command() = delete;

        std::string _name;
        std::string _desc;
        MessageHandler _handler;
        bool _requireOwner;
};

class CommandCategory
{
    public:
        CommandCategory(std::string name, std::vector<Command*> cmds);
        inline std::string getName() { return _name; }
        Command* findCommand(std::string name);

    private:
        CommandCategory() = delete;
        ~CommandCategory() = delete;

        std::string _name;
        std::unordered_map<std::string, Command*> _commands;
};

class CommandManager
{
    public:
        static CommandManager* get()
        {
            if (_instance == nullptr)
                _instance = new CommandManager();
            return _instance;
        }

        Command* findCommand(std::string name);
        inline void registerCategory(CommandCategory* cat) { _categories[cat->getName()] = cat; }

        inline std::string getPrefix() { return _prefix; }
        inline void setPrefix(std::string prefix) { _prefix = prefix; }

        inline void handleMessage(Client* client, Message* msg)
        {
            if (!msg->content.starts_with(_prefix))
                return;

            std::string trimmed = msg->content.substr(_prefix.length());
            std::istringstream stream(trimmed);

            std::string cmdName;
            stream >> cmdName;

            Command* cmd = findCommand(cmdName);
            if (!cmd)
                return;

            if (cmd->requiresOwner())
            {
                auto guild = client->getGuild(msg->guild_id.value());
                if (guild->owner_id != msg->author->id)
                    return;
            }

            std::vector<std::string> args = {};
            std::string arg;
            while (stream >> arg)
                args.push_back(arg);

            auto channel = client->getChannel(msg->channel_id.value());
            cmd->run(client, channel, msg, &args);
        }

    private:
        CommandManager(){};
        ~CommandManager() = delete;

        std::string _prefix;

        std::unordered_map<std::string, CommandCategory*> _categories;

        inline static CommandManager* _instance = nullptr;
};