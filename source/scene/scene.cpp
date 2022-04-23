#include "scene.hpp"

namespace QDUEngine
{
    void Scene::addGameObject(GameObject& gameObject)
    {
        m_gameObjects.push_back(gameObject);
    }

    void Scene::start(char *name, Vector2D& windowSize)
    {
        m_window.start(name, windowSize, m_input);
        m_input.start();
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
        m_window.end();
    }
}
