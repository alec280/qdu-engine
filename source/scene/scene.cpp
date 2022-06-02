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
        auto mainInput = gameObject->getInputComponent();
        auto mainVisual = gameObject->getVisualComponent();
        mainInput->setMain(true);
        mainVisual->setMain(true);
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

    SceneData Scene::getData() {
        std::map<std::string, std::string> map{};
        std::map<std::string, std::string> objects{};
        for (auto& object : m_gameObjects) {
            if (object == m_mainObject) {
                continue;
            }
            auto jsonData = object->getData();
            map[object->getVisualComponent()->getPosition().toString()] = jsonData.first;
            objects[jsonData.first] = jsonData.second;
        }
        std::map<std::string, std::map<std::string, std::string>> transitions{};
        for (auto& transition : m_transitions) {
            std::map<std::string, std::string> temp{};
            temp["target"] = transition.first;
            temp["at"] = transition.second.second.toString();
            transitions[transition.second.first.toString()] = temp;
        }
        return {map, objects, transitions};
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
