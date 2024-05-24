#include "commandmanager.h"

CommandCategory::CommandCategory(std::string name, std::vector<Command*> cmds) : _name(name)
{
    for (auto cmd : cmds)
        _commands[cmd->getName()] = cmd;
    CommandManager::get()->registerCategory(this);
}

Command* CommandCategory::findCommand(std::string name)
{
    auto it = _commands.find(name);
    if (it != _commands.end())
        return it->second;
    return nullptr;
}

Command* CommandManager::findCommand(std::string name)
{
    for (auto cat : _categories)
    {
        Command* cmd = cat.second->findCommand(name);
        if (cmd)
            return cmd;
    }
    return nullptr;
}
