#include "scene.hpp"

namespace QDUEngine
{
    void Scene::addGameObject(VisualComponent& gameObject)
    {
        auto object = std::make_shared<VisualComponent>(gameObject);
        m_gameObjects.push_back(object);
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
        //auto visualComponents = std::vector<std::shared_ptr<VisualComponent>> (2);
        /*
        for (auto& gameObject : m_gameObjects) {
            auto component = std::make_shared<VisualComponent>(gameObject.getVisualComponent());
            //std::cout << gameObject.getVisualComponent().getPosition() << std::endl;
            visualComponents.push_back(component);
        }
        */
        m_window.update(m_gameObjects);
    }

    void Scene::end()
    {
        for (auto& gameObject : m_gameObjects) {
            gameObject->getGraphNodePtr()->clear();
        }
        m_window.end();
    }

    VisualComponent Scene::getCube()
    {
        auto component = m_window.getCube();
        return component;
    }
}
