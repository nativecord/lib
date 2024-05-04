#pragma once

#include <list>
#include <unordered_map>

template <typename KeyType, typename ValueType> class Cache
{
    public:
        using KeyValuePair = std::pair<KeyType, std::unique_ptr<ValueType>>;

        Cache(size_t maxSize) : _maxSize(maxSize){};

        void insert(const KeyType& key, std::unique_ptr<ValueType> value)
        {
            if (_data.size() == _maxSize)
            {
                _indexes.erase(_data.front().first);
                _data.pop_front();
            }
            _data.emplace_back(key, std::move(value));
            _indexes[key] = std::prev(_data.end());
        }

        bool remove(const KeyType& key)
        {
            auto it = _indexes.find(key);
            if (it != _indexes.end())
            {
                _data.erase(it->second);
                _indexes.erase(key);
                return true;
            }
            return false;
        }

        bool contains(const KeyType& key)
        {
            auto it = _indexes.find(key);
            if (it != _indexes.end())
                return true;
            return false;
        }

        size_t getLimit() { return _maxSize; }
        void setLimit(size_t newSize)
        {
            _maxSize = newSize;
            if (newSize < _data.size())
                for (int i = newSize; i < _data.size(); i++)
                    _data.pop_front();
        }

    private:
        std::unordered_map<KeyType, typename std::list<KeyValuePair>::iterator> _indexes;
        std::list<KeyValuePair> _data;
        size_t _maxSize;
};
