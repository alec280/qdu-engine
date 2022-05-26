#pragma once
#include <vector>
#include "../game_object/game_object.hpp"
#include "../grafica/root_directory.h"
#include "window.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

namespace QDUEngine
{
    class Scene {
        friend class Application;
    public:
        void addGameObject(GameObject& gameObject);
        std::shared_ptr<VisualComponent> getCube();
        std::shared_ptr<VisualComponent> getCube(float r, float g, float b);
        std::shared_ptr<VisualComponent> getTexturedCube(const char* texturePath);
        virtual void userStart() = 0;
        std::shared_ptr<InputComponent> getInputComponent();
        void addVisualComponent(GameObject &gameObject);
        void setInputComponent(InputComponent* inputComponent);
        void fromJSON(const char* path);
        //std::shared_ptr<GameObject> getGameObject(int idx);
        void bindCursorButton(const char* key, const char* action);
        void bindKey(const char* key, const char* action);
        void bindJoystick(const char* key, const char* action);
    private:
        void start(const char* name, const Vector2D& windowSize);
        void update(float delta);
        void end();
        void clear();
        InputComponent* m_inputComponent;
        Window m_window{};
        Input m_input{};
    };
}