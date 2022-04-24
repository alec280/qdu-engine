#include "scene.hpp"

namespace QDUEngine
{
    void Scene::addGameObject(GameObject& gameObject)
    {
        m_gameObjects.push_back(std::make_shared<GameObject>(gameObject));
        m_window.m_visualComponents.push_back(std::make_shared<VisualComponent>(gameObject.getVisualComponent()));
    }

    void Scene::start(char *name, Vector2D& windowSize)
    {
        m_window.start(name, windowSize, m_input);
        m_input.start();
        userStart();
        while (!m_window.shouldClose()) {
            update(0);
        }
        end();
    }

    void Scene::update(float delta)
    {
        m_input.update();
        m_window.update();
    }

    void Scene::end()
    {
        for (auto& gameObject : m_gameObjects) {
            gameObject->getVisualComponent().getGraphNodePtr()->clear();
        }
        m_window.end();
    }

    VisualComponent Scene::getCube()
    {
        auto component = m_window.getCube();
        return component;
    }
}
