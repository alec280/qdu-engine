#pragma once
#include <vector>
#include "../game_object/game_object.hpp"
#include "window.hpp"

namespace QDUEngine
{
    class Scene {
        friend class Application;
    public:
        void addGameObject(GameObject& gameObject);
        VisualComponent getCube();
    private:
        void start(char *name, Vector2D& windowSize);
        void update(float delta);
        void end();
        std::vector<GameObject> m_gameObjects;
        Window m_window{};
        Input m_input{};
    };
}