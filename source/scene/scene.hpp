#pragma once
#include <filesystem>
#include "../game_object/game_object.hpp"
#include "../grafica/root_directory.h"

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
        SceneData getData();
        std::vector<std::shared_ptr<GameObject>> getObjects();
    private:
        std::vector<std::shared_ptr<GameObject>> m_gameObjects{};
        std::vector<std::shared_ptr<GameObject>> m_gameObjectsQueue{};
        std::shared_ptr<GameObject> m_mainObject = nullptr;
        std::string m_name;
        std::string m_source;
        std::map<std::string, std::pair<Vector2D, Vector2D>> m_transitions{};
        void addTransition(std::string& toScene, const Vector2D& fromTile, const Vector2D& toTile);
        void end();
        void start(const char* name, const Vector2D& windowSize);
        void update();
    };
}