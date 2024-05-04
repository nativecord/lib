#pragma once

#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <vector>

/*
    to-do:
        add way to unregister listeners
*/

template <typename T> struct function_traits : public function_traits<decltype(&T::operator())>
{
};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...) const>
{
        using argsTuple = std::tuple<Args...>;
};

template <typename F> using function_info = function_traits<decltype(&F::operator())>;

struct EventInfo
{
    public:
        std::vector<size_t> _types;
        std::vector<void*> _listeners;
};

class EventEmitter
{
    private:
        template <typename... T> std::vector<size_t> getTypeHashCodes() { return {typeid(T).hash_code()...}; }

        template <typename Tuple, size_t Index = 0> void getTupleTypeHashCodes(std::vector<size_t>* hashList)
        {
            if constexpr (Index < std::tuple_size_v<Tuple>)
            {
                hashList->push_back(typeid(typename std::tuple_element<Index, Tuple>::type).hash_code());
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
            if (!_events.contains(eventName))
                throw new std::logic_error("attempted to fire non-existing event");

            auto it = _events.find(eventName);
            std::unique_ptr<EventInfo>& eventInfo = it->second;
            std::vector<size_t> argTypes = getTypeHashCodes<Args...>();

            if (argTypes != eventInfo->_types)
                throw new std::logic_error("event args do not match args passed to fireEvent");

            for (auto functionPointer : eventInfo->_listeners)
            {
                auto fn = reinterpret_cast<void (*)(Args...)>(functionPointer);
                fn(std::forward<Args>(args)...);
            }
        }

        template <typename F> inline void on(std::string eventName, F listenerFn)
        {
            if (!_events.contains(eventName))
                throw new std::logic_error("attempted to register listener for non-existeng event");

            auto it = _events.find(eventName);
            std::unique_ptr<EventInfo>& eventInfo = it->second;

            using FI = function_info<decltype(listenerFn)>;
            std::vector<size_t> listenerArgHashes{};
            getTupleTypeHashCodes<typename FI::argsTuple>(&listenerArgHashes);

            if (eventInfo->_types != listenerArgHashes)
                throw new std::logic_error("listener function signature does not match event");

            eventInfo->_listeners.push_back(reinterpret_cast<void*>(+listenerFn));
        }

    private:
        std::unordered_map<std::string, std::unique_ptr<EventInfo>> _events;
};