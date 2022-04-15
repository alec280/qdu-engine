#pragma once
#include <map>
#include "vector_2d.hpp"

namespace QDUEngine
{
    template<class K, class V>
    class Dictionary {
    public:
        Dictionary(K keyType, V valueType);
        void add(const K& key, const V& value);
        V get(const K& key);
        bool isEmpty();
        int getSize();
        void clear();
        K* getKeys();
        V* getValues();
    private:
        std::map<K,V> m_map{};
    };
}

