#pragma once
#include <filesystem>
#include "../game_object/game_object.hpp"
#include "../grafica/root_directory.h"
#include "window.hpp"

namespace QDUEngine
{
    struct SceneData {
        std::map<std::string, std::string> map;
        std::map<std::string, std::string> objects;
        std::map<std::string, std::map<std::string, std::string>> transitions;
    };

    class Scene {
        friend class Application;
    public:
        void addGameObject(GameObject& gameObject);
        void addMainObject(GameObject& gameObject);
        std::shared_ptr<VisualComponent> getCube();
        std::shared_ptr<VisualComponent> getCube(float r, float g, float b);
        std::shared_ptr<VisualComponent> getTexturedCube(const char* texturePath);
        std::shared_ptr<VisualComponent> getTexturedCube(const char* texturePath, const char* name);
        virtual void userStart() = 0;
        std::shared_ptr<InputComponent> getInputComponent();
        void addInputComponent(GameObject& gameObject);
        void addVisualComponent(GameObject& gameObject);
        void setInputComponent(InputComponent* inputComponent);
        void fromJSON(const char* path);
        SceneData getData();
        void bindCursorButton(const char* key, const char* action);
        void bindKey(const char* key, const char* action);
        void bindJoystick(const char* key, const char* action);
        void saveJSON();
    private:
        void start(const char* name, const Vector2D& windowSize);
        void update(float delta);
        void end();
        void clear();
        void preloadJSON(const char* path);
        void addTransition(std::string& toScene, const Vector2D& fromTile, const Vector2D& toTile);
        InputComponent* m_inputComponent;
        std::shared_ptr<GameObject> m_mainObject = nullptr;
        std::string m_name;
        Window m_window{};
        Input m_input{};
        char* m_tempDir;
        std::map<std::string, std::pair<Vector2D, Vector2D>> m_transitions{};
    };
}