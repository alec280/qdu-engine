#include "map.hpp"

namespace QDUEngine
{
    const Vector2D& Map::getSize()
    {
        return m_size;
    }

    void Map::add(const GameObject& gameObject, const Vector2D &position)
    {
        if (!contains(position)) {
            return;
        }
        if (m_gameObjects.has(position)) {
            m_gameObjects.remove(position);
        }
        m_gameObjects.set(position, gameObject);
    }

    bool Map::contains(const Vector2D &position) const
    {
        return position.x >= 0 && position.y >= 0 && position.x < m_size.x && position.y < m_size.y;
    }

    Map::Map(const Vector2D &size) : m_size(size) {}

    void Map::update(float delta)
    {
        auto gameObjects = getGameObjects();
        for (int i = 0; i < m_gameObjects.getSize(); i++) {
            gameObjects[i].update(*this);
        }
    }

    GameObject* Map::getGameObjects()
    {
        return m_gameObjects.getValues();
    }
}