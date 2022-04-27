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
        std::shared_ptr<VisualComponent> getCube();
        std::shared_ptr<VisualComponent> getCube(float r, float g, float b);
        virtual void userStart() = 0;
        std::shared_ptr<InputComponent> getInputComponent();
        void addVisualComponent(GameObject &gameObject);
        void setInputComponent(InputComponent* inputComponent);
        std::shared_ptr<GameObject> getGameObject(int idx);
    private:
        void start(const char* name, const Vector2D& windowSize);
        void bindCursorButton(const char* key, const char* action);
        void bindKey(const char* key, const char* action);
        void bindJoystick(const char* key, const char* action);
        void update(float delta);
        void end();
        InputComponent* m_inputComponent;
        Window m_window{};
        Input m_input{};
    };
}