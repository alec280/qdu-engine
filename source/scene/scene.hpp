#pragma once
#include <filesystem>
#include "../game_object/game_object.hpp"
#include "../grafica/root_directory.h"
#include "navigation_mesh.hpp"

namespace QDUEngine
{
    class Scene {
        friend class Application;
    public:
        void addGameObject(GameObject& gameObject);
        void addMainObject(GameObject& gameObject);
        std::shared_ptr<GameObject> getMainObject();
        std::string getName();
        std::shared_ptr<NavigationMesh> getNavigationMesh();
        nlohmann::json getData();
        std::vector<std::shared_ptr<GameObject>> getObjects();
        void setNavigationMesh(std::shared_ptr<NavigationMesh>& gameObject);
    private:
        std::vector<std::shared_ptr<GameObject>> m_gameObjects{};
        std::vector<std::shared_ptr<GameObject>> m_gameObjectsQueue{};
        std::shared_ptr<GameObject> m_mainObject = nullptr;
        std::string m_name;
        std::shared_ptr<NavigationMesh> m_navigation = nullptr;
        std::string m_source;
        std::map<std::string, std::pair<Vector2D, Vector2D>> m_transitions{};
        void addMainObject(std::shared_ptr<GameObject>& gameObject);
        void addTransition(std::string& toScene, const Vector2D& fromTile, const Vector2D& toTile);
        void end();
        std::shared_ptr<GameObject> getById(std::string& objectId);
        void update();
    };
}