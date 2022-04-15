#pragma once
#include <map>
#include "../core/dictionary.hpp"
#include "../entity/entity.hpp"

namespace QDUEngine
{
    class Map {
    public:
        Map(const Map& map) = delete;
        Map& operator=(const Map& map) = delete;
        const Vector2D& getSize() const;
        void add(const Entity& entity, const Vector2D& position);
        bool contains(const Vector2D& position) const;
    private:
        Vector2D m_size;
        Dictionary<Vector2D, Entity> m_entities;
    };
}