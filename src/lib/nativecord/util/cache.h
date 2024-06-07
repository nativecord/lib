#pragma once

#include <list>
#include <mutex>
#include <unordered_map>

template <typename KeyType, typename ValueType> class Cache
{
    public:
        using KeyValuePair = std::pair<KeyType, std::shared_ptr<ValueType>>;

        Cache(size_t maxSize) : _maxSize(maxSize), _mutex(){};

        void insert(const KeyType& key, std::shared_ptr<ValueType> value)
        {
            std::lock_guard<std::mutex> lock(_mutex);

            auto it = _indexes.find(key);
            if (it != _indexes.end())
            {
                _data.splice(_data.end(), _data, it->second);
                it->second->second = value;
            }
            else
            {
                if (_data.size() == _maxSize)
                {
                    _indexes.erase(_data.front().first);
                    _data.pop_front();
                }
                _data.emplace_back(key, value);
                _indexes[key] = std::prev(_data.end());
            }
        }

        bool remove(const KeyType& key)
        {
            std::lock_guard<std::mutex> lock(_mutex);

            auto it = _indexes.find(key);
            if (it != _indexes.end())
            {
                _data.erase(it->second);
                _indexes.erase(it);
                return true;
            }
            return false;
        }

        bool contains(const KeyType& key)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            return _indexes.find(key) != _indexes.end();
        }

        std::shared_ptr<ValueType> get(const KeyType& key)
        {
            std::lock_guard<std::mutex> lock(_mutex);

            auto it = _indexes.find(key);
            if (it != _indexes.end())
            {
                _data.splice(_data.end(), _data, it->second);
                return it->second->second;
            }
            return nullptr;
        }

        size_t getLimit() { return _maxSize; }
        void setLimit(size_t newSize)
        {
            std::lock_guard<std::mutex> lock(_mutex);

            _maxSize = newSize;
            while (_data.size() > _maxSize)
            {
                _indexes.erase(_data.front().first);
                _data.pop_front();
            }
        }

        std::shared_ptr<ValueType>& operator[](const KeyType& key)
        {
            std::lock_guard<std::mutex> lock(_mutex);

            auto it = _indexes.find(key);
            if (it == _indexes.end())
            {
                if (_data.size() == _maxSize)
                {
                    _indexes.erase(_data.front().first);
                    _data.pop_front();
                }
                _data.emplace_back(key, std::make_shared<ValueType>());
                _indexes[key] = std::prev(_data.end());
                it = _indexes.find(key);
            }

            _data.splice(_data.end(), _data, it->second);
            return it->second->second;
        }

        const std::vector<KeyValuePair> getContents() const
        {
            std::lock_guard<std::mutex> lock(_mutex);
            return std::vector<KeyValuePair>(_data.begin(), _data.end());
        }
    private:
        std::unordered_map<KeyType, typename std::list<KeyValuePair>::iterator> _indexes;
        std::list<KeyValuePair> _data;
        size_t _maxSize;
        mutable std::mutex _mutex;
};
