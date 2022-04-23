#pragma once
#include <map>
#include "../game_object/game_object.hpp"
#include "../core/dictionary.hpp"

namespace QDUEngine
{
    class GameObject;
    class Map {
    public:
        explicit Map(const Vector2D& size);
        Map(const Map& map) = delete;
        Map& operator=(const Map& map) = delete;
        const Vector2D& getSize();
        void add(const GameObject& gameObject, const Vector2D& position);
        bool contains(const Vector2D& position) const;
        GameObject* getGameObjects();
    private:
        void update(float delta);
        Vector2D m_size;
        Dictionary<Vector2D, GameObject> m_gameObjects{};
    };
}