#include "scene.hpp"

namespace QDUEngine
{
    void Scene::addGameObject(GameObject& gameObject)
    {
        m_gameObjectsQueue.push_back(std::make_shared<GameObject>(gameObject));
    }

    void Scene::addMainObject(GameObject& gameObject)
    {
        auto objectPtr = std::make_shared<GameObject>(gameObject);
        addMainObject(objectPtr);
    }

    void Scene::addMainObject(std::shared_ptr<GameObject>& gameObject)
    {
        if (m_mainObject != nullptr) {
            return;
        }
        m_mainObject = gameObject;
        m_gameObjectsQueue.push_back(m_mainObject);
    }

    void Scene::addTransition(std::string& toScene, const Vector2D& fromTile, const Vector2D& toTile)
    {
        m_transitions[toScene] = std::pair<Vector2D, Vector2D>(fromTile, toTile);
    }

    std::vector<std::shared_ptr<GameObject>> Scene::getObjects()
    {
        return m_gameObjects;
    }

    void Scene::end()
    {
        for (auto& object : m_gameObjectsQueue) {
            object->end();
        }
        for (auto& object : m_gameObjects) {
            object->end();
        }
        m_gameObjectsQueue.clear();
        m_gameObjects.clear();
        m_mainObject = nullptr;
    }

    nlohmann::json Scene::getData() {
        nlohmann::json data{{"transitions", {}}, {"objects", nlohmann::json::array()}};
        for (auto& object : m_gameObjects) {
            if (object == m_mainObject) {
                continue;
            }
            data["objects"].push_back(object->getData());
        }
        for (auto& transition : m_transitions) {
            auto vectors = transition.second;
            data["transitions"][transition.first] = {
                    {"fromX", vectors.first.x},
                    {"fromY", vectors.first.y},
                    {"toX", vectors.second.x},
                    {"toY", vectors.second.y}
            };
        }
        return data;
    }

    void Scene::update()
    {
        while (!m_gameObjectsQueue.empty()) {
            auto component = m_gameObjectsQueue.front();
            m_gameObjects.push_back(component);
            m_gameObjectsQueue.erase(m_gameObjectsQueue.begin());
        }
    }
}
