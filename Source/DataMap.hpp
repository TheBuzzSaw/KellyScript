#ifndef DATAMAP_HPP_KELLY
#define DATAMAP_HPP_KELLY

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cassert>

template<class T> struct View
{
    T* data;
    int count;
    
    inline T* begin() const { return data; }
    inline T* end() const { return data + count; }
    
    operator View<const T>() { return {data, count}; }
};

template<typename K, typename V> class DataMap
{
    static constexpr int KeyValueSize = sizeof(K) + sizeof(V);

    void* _block;
    int _capacity;
    int _count;

    void Expand(int capacity)
    {
        auto proposedCapacity = std::max<int>(capacity, 8);

        void* block = realloc(_block, proposedCapacity * KeyValueSize);
        assert(block != nullptr);
        _block = block;

        memmove(
            (K*)_block + proposedCapacity,
            (K*)_block + _capacity,
            sizeof(V) * (size_t)_count);

        _capacity = proposedCapacity;
    }

    V* FindValue(K key) const noexcept
    {
        auto keys = (K*)_block;
        auto endKey = keys + _count;

        auto k = std::lower_bound(keys, endKey, key);

        if (k != endKey && *k == key)
        {
            auto values = (V*)(keys + _capacity);
            auto position = k - keys;
            return values + position;
        }

        return nullptr;
    }

    V& Insert(K key, int position)
    {
        if (_count == _capacity) Expand(_capacity * 2);
        auto keys = (K*)_block;
        auto values = (V*)(keys + _capacity);

        auto moveCount = _count++ - position;
        auto keySlot = keys + position;
        memmove(keySlot + 1, keySlot, moveCount * sizeof(K));
        *keySlot = key;

        auto valueSlot = values + position;
        memmove(valueSlot + 1, valueSlot, moveCount * sizeof(V));
        return *valueSlot;
    }

public:

    DataMap() noexcept
        : _block(nullptr)
        , _capacity(0)
        , _count(0)
    {
    }

    DataMap(DataMap&& other) noexcept
        : _block(other._block)
        , _capacity(other._capacity)
        , _count(other._count)
    {
        other._block = nullptr;
        other._capacity = 0;
        other._count = 0;
    }

    DataMap(const DataMap& other)
        : DataMap()
    {
        if (other._count > 0)
        {
            Expand(other._count);
            _count = other._count;

            memcpy(
                _block,
                other._block,
                (size_t)_count * sizeof(K));

            memcpy(
                (K*)_block + _capacity,
                (K*)other._block + other._capacity,
                (size_t)_count * sizeof(V));
        }
    }

    ~DataMap() noexcept
    {
        free(_block);
    }

    DataMap& operator=(DataMap&& other) noexcept
    {
        if (this != &other)
        {
            free(_block);

            _block = other._block;
            _capacity = other._capacity;
            _count = other._count;

            other._block = nullptr;
            other._capacity = 0;
            other._count = 0;
        }

        return *this;
    }

    DataMap& operator=(const DataMap& other)
    {
        if (this != &other)
        {
            if (_capacity < other._count)
            {
                _count = 0;
                Expand(other._count);
            }

            _count = other._count;

            memcpy(
                _block,
                other._block,
                (size_t)_count * sizeof(K));

            memcpy(
                (K*)_block + _capacity,
                (K*)other._block + other._capacity,
                (size_t)_count * sizeof(V));
        }

        return *this;
    }

    int RawSize() const noexcept
    {
        return _capacity * KeyValueSize;
    }

    int Capacity() const noexcept
    {
        return _capacity;
    }

    int Count() const noexcept
    {
        return _count;
    }

    void Clear() noexcept
    {
        _count = 0;
    }

    V* TryGetValue(K key) noexcept
    {
        return FindValue(key);
    }

    const V* TryGetValue(K key) const noexcept
    {
        return FindValue(key);
    }

    V& AlwaysGet(K key)
    {
        auto keys = (K*)_block;
        auto endKey = keys + _count;

        auto k = std::lower_bound(keys, endKey, key);
        auto position = k - keys;

        if (k != endKey && *k == key)
        {
            auto values = (V*)(keys + _capacity);
            return values[position];
        }

        return Insert(key, position);
    }

    bool Add(K key, V value)
    {
        auto keys = (K*)_block;
        auto endKey = keys + _count;

        auto k = std::lower_bound(keys, endKey, key);

        if (k != endKey && *k == key) return false;

        auto position = k - keys;
        Insert(key, position) = value;
        return true;
    }

    void Set(K key, V value)
    {
        auto keys = (K*)_block;
        auto endKey = keys + _count;

        auto k = std::lower_bound(keys, endKey, key);
        auto position = k - keys;

        if (k != endKey && *k == key)
        {
            auto values = (V*)(keys + _capacity);
            values[position] = value;
        }
        else
        {
            Insert(key, position) = value;
        }
    }

    bool Remove(K key) noexcept
    {
        auto keys = (K*)_block;
        auto endKey = keys + _count;

        auto k = std::lower_bound(keys, endKey, key);

        if (k != endKey && *k == key)
        {
            auto values = (V*)(keys + _capacity);
            auto position = k - keys;
            auto moveCount = --_count - position;

            memmove(k, k + 1, (size_t)moveCount * sizeof(K));

            auto valueSlot = values + position;
            memmove(valueSlot, valueSlot + 1, moveCount * sizeof(V));
            return true;
        }

        return false;
    }

    void Reserve(int capacity)
    {
        if (capacity > _capacity) Expand(capacity);
    }

    View<V> Values() noexcept
    {
        return { (V*)((K*)_block + _capacity), _count };
    }

    View<const K> Keys() const noexcept
    {
        return { (K*)_block, _count };
    } 

    View<const V> Values() const noexcept
    {
        return { (V*)((K*)_block + _capacity), _count };
    }
};


#endif

