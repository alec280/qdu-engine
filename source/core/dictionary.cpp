#include "dictionary.hpp"

namespace QDUEngine
{
    template<class K, class V>
    Dictionary<K, V>::Dictionary(K keyType, V valueType) = default;

    template<class K, class V>
    void Dictionary<K, V>::add(const K& key, const V& value)
    {
        m_map.insert(std::pair<K, V>(key, value));
    }

    template<class K, class V>
    V Dictionary<K, V>::get(const K &key)
    {
        return m_map.at(key);
    }

    template<class K, class V>
    bool Dictionary<K, V>::isEmpty()
    {
        return m_map.empty();
    }

    template<class K, class V>
    int Dictionary<K, V>::getSize()
    {
        return m_map.size();
    }

    template<class K, class V>
    void Dictionary<K, V>::clear()
    {
        m_map.clear();
    }

    template<class K, class V>
    K* Dictionary<K, V>::getKeys()
    {
        static K keys[m_map.size()];
        int count = 0;
        for(auto it = m_map.begin(); it != m_map.end(); ++it) {
            keys[count++] = it->first;
        }
        return keys;
    }

    template<class K, class V>
    V* Dictionary<K, V>::getValues()
    {
        static V values[m_map.size()];
        int count = 0;
        for(auto it = m_map.begin(); it != m_map.end(); ++it) {
            values[count++] = it->second;
        }
        return values;
    }

    template<class K, class V>
    bool Dictionary<K, V>::has(const K &key)
    {
        auto it = m_map.find(key);
        return it != m_map.end();
    }

    template<class K, class V>
    void Dictionary<K, V>::remove(const K &key)
    {
        if (!has(key)) {
            return;
        }
        m_map.erase(key);
    }
}