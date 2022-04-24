#pragma once
#include <map>

namespace QDUEngine
{
    template<class K, class V>
    class Dictionary {
    public:
        Dictionary();
        void set(const K& key, const V& value);
        V get(const K& key);
        bool isEmpty();
        int getSize();
        void clear();
        bool has(const K& key);
        void remove(const K& key);
        K* getKeys();
        V* getValues();
    private:
        std::map<K,V> m_map{};
    };
}

