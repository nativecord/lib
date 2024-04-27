#pragma once

#include "nativecord/util/assert.h"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/*
    to-do:
        test for safety?
        try to get as many compile-time asserts as possible
*/

template <typename T> struct function_traits : public function_traits<decltype(&T::operator())>
{
};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...) const>
{
        using argTypes = std::tuple<Args...>;
};

template <typename F> using function_info = function_traits<decltype(&F::operator())>;

// not using a pair but using this struct instead because i plan on storing more info
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
        template <typename Tuple, size_t Index = 0> void getTupleTypeHashCodes(std::vector<size_t>* hashList)
        {
            if constexpr (Index < std::tuple_size_v<Tuple>)
            {
                hashList->push_back(typeid(std::tuple_element<Index, Tuple>::type).hash_code());
                getTupleTypeHashCodes<Tuple, Index + 1>(hashList);
            }
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

        template <typename Func> inline void on(std::string eventName, Func func)
        {
            ASSERT(_events.contains(eventName), "attempted to register callback for non-existing event");
            auto it = _events.find(eventName);
            ASSERT(it != _events.end(), "couldn't find event");

            std::unique_ptr<EventInfo>& event = it->second;

            using FI = function_info<decltype(func)>;
            std::vector<size_t> funcTypeHashes{};
            getTupleTypeHashCodes<typename FI::argTypes>(&funcTypeHashes);

            ASSERT(event->_types.size() == funcTypeHashes.size(), "argument count did not match");
            ASSERT(funcTypeHashes == event->_types, "func type does not match event type");

            event->_functions.push_back(func);
        }

    private:
        std::unordered_map<std::string, std::unique_ptr<EventInfo>> _events;
};