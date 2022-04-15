#pragma once
#include <map>
#include "../entity/entity.hpp"

namespace QDUEngine
{
    class Map {
    public:
        explicit Map(const Vector2D& size);
        Map(const Map& map) = delete;
        Map& operator=(const Map& map) = delete;
        const Vector2D& getSize();
        void add(const Entity& entity, const Vector2D& position);
        bool contains(const Vector2D& position) const;
        Entity* getEntities();
    private:
        void update(float delta);
        Vector2D m_size;
        Dictionary<Vector2D, Entity> m_entities{};
    };
}