#include "map.hpp"

namespace QDUEngine
{
    const Vector2D& Map::getSize()
    {
        return m_size;
    }

    void Map::add(const Entity &entity, const Vector2D &position)
    {
        if (!contains(position)) {
            return;
        }
        if (m_entities.has(position)) {
            m_entities.remove(position);
        }
        m_entities.set(position, entity);
    }

    bool Map::contains(const Vector2D &position) const
    {
        return position.x >= 0 && position.y >= 0 && position.x < m_size.x && position.y < m_size.y;
    }

    Map::Map(const Vector2D &size) : m_size(size) {}

    void Map::update(float delta)
    {
        auto entities = getEntities();
        for (int i = 0; i < m_entities.getSize(); i++) {
            entities[i].update(delta);
        }
    }

    Entity* Map::getEntities()
    {
        return m_entities.getValues();
    }
}