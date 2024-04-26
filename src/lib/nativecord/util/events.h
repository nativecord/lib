#pragma once

#include "nativecord/util/assert.h"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/*
    to-do:
        deduct function args from Func template instead of requiring args manually (::on)
        improve safety
        see if more asserts can be made static
*/

// not using a pair and using this struct because i plan on storing more info
struct EventInfo
{
    public:
        std::vector<size_t> _types;
        std::vector<void*> _functions;
};

class EventEmitter
{
    private:
        template <typename... T> std::vector<size_t> getTypeHashCodes()
        {
            return {typeid(T).hash_code()...};
        }

    public:
        template <typename... Args> inline void registerEvent(std::string eventName)
        {
            std::vector<size_t> types = getTypeHashCodes<Args...>();

            auto info = std::make_unique<EventInfo>();
            info->_types = types;

            _events.insert(std::make_pair(eventName, std::move(info)));
        }

        template <typename... Args> inline void fireEvent(std::string eventName, Args&&... args)
        {
            ASSERT(_events.contains(eventName), "attempted to fire non-existing event");
            auto it = _events.find(eventName);
            ASSERT(it != _events.end(), "couldn't find eventinfo");
            std::unique_ptr<EventInfo>& eventInfo = it->second;

            std::vector<size_t> types = getTypeHashCodes<Args...>();

            ASSERT(eventInfo->_types == types, "type mismatch during event fire");

            using FuncType = std::function<void(Args...)>;
            for (auto funcPtr : eventInfo->_functions)
            {
                auto fn = reinterpret_cast<void (*)(Args...)>(funcPtr);
                fn(std::forward<Args>(args)...);
            }
        }

        template <typename... Args, typename Func> inline void on(std::string eventName, Func func)
        {
            static_assert(std::is_invocable_r<void, Func, Args...>::value,
                          "listener signature does not match event handler signature.");
            static_assert(std::is_same_v<void, std::invoke_result_t<Func, Args...>>, "listener must return void");

            ASSERT(_events.contains(eventName), "attempted to register callback for non-existing event");
            auto it = _events.find(eventName);
            ASSERT(it != _events.end(), "couldn't find eventinfo");
            std::unique_ptr<EventInfo>& eventInfo = it->second;

            std::vector<size_t> types = getTypeHashCodes<Args...>();
            ASSERT(eventInfo->_types == types, "type mismatch during event listener registration");

            eventInfo->_functions.push_back(func);
        }

    private:
        std::unordered_map<std::string, std::unique_ptr<EventInfo>> _events;
};