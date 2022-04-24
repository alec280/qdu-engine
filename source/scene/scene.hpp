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
        VisualComponent getCube(float r, float g, float b);
        virtual void userStart() = 0;
    private:
        void start(
                const char *name,
                const Vector2D &windowSize,
                std::map<const char*, const char*>& keyBindings,
                std::map<const char*, const char*>& joystickBindings
                );
        void update(float delta);
        void end();
        std::vector<std::shared_ptr<GameObject>> m_gameObjects;
        Window m_window{};
        Input m_input{};
    };
}