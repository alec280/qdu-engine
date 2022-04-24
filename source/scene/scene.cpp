#include "scene.hpp"

namespace QDUEngine
{
    void Scene::addGameObject(GameObject& gameObject)
    {
        m_gameObjects.push_back(std::make_shared<GameObject>(gameObject));
        m_input.m_inputComponents.push_back(std::make_shared<InputComponent>(gameObject.getInputComponent()));
        m_window.m_visualComponents.push_back(std::make_shared<VisualComponent>(gameObject.getVisualComponent()));
    }

    void Scene::start(const char *name, const Vector2D &windowSize, std::map<const char*, const char*>& bindings)
    {
        m_window.start(name, windowSize, m_input);
        m_input.start(bindings);
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
        auto component = m_window.getCube(1, 1, 1);
        return component;
    }

    VisualComponent Scene::getCube(float r, float g, float b)
    {
        auto component = m_window.getCube(r, g, b);
        return component;
    }
}
