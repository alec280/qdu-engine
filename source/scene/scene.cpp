#include "scene.hpp"

namespace QDUEngine
{
    void Scene::addGameObject(GameObject& gameObject)
    {
        m_input.m_inputComponents.push_back(gameObject.getInputComponent());
        m_window.m_visualComponents.push_back(gameObject.getVisualComponent());
    }

    void Scene::addVisualComponent(GameObject& gameObject)
    {
        m_window.m_visualComponents.push_back(gameObject.getVisualComponent());
    }

    std::shared_ptr<InputComponent> Scene::getInputComponent()
    {
        return std::shared_ptr<InputComponent>(m_inputComponent);
    }

    void Scene::setInputComponent(InputComponent* inputComponent)
    {
        m_inputComponent = inputComponent;
        m_input.m_appInputComponent = getInputComponent();
    }

    void Scene::start(const char* name, const Vector2D& windowSize)
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
        bool transition = false;
        for (auto& component : m_window.m_visualComponents) {
            if (component->getPosition() == Vector(0, -7) && component->isMain()) {
                transition = true;
            }
        }
        if (transition) {
            fromJSON("examples/data/warehouse.json");
        }
    }

    void Scene::end()
    {
        m_window.clear();
        m_window.end();
    }

    std::shared_ptr<VisualComponent> Scene::getCube()
    {
        auto component = m_window.getCube(1, 1, 1);
        return component;
    }

    std::shared_ptr<VisualComponent> Scene::getCube(float r, float g, float b)
    {
        auto component = m_window.getCube(r, g, b);
        return component;
    }

    std::shared_ptr<VisualComponent> Scene::getTexturedCube(const char* texturePath)
    {
        return m_window.getTexturedCube(texturePath);
    }

    void Scene::bindCursorButton(const char* key, const char* action)
    {
        m_input.m_cursorBindings[std::string(key)] = action;
        m_input.m_cursorActions[std::string(action)] = 0;
    }

    void Scene::bindKey(const char* key, const char* action)
    {
        m_input.m_keyBindings[std::string(key)] = action;
        m_input.m_actions[std::string(action)] = 0;
    }

    void Scene::bindJoystick(const char* key, const char* action)
    {
        m_input.m_joystickBindings[std::string(key)] = action;
        m_input.m_actions[std::string(action)] = 0;
    }

    void Scene::fromJSON(const char* path)
    {
        clear();
        m_window.fromJSON(path);
    }

    void Scene::preloadJSON(const char* path)
    {
        nlohmann::json jf = nlohmann::json::parse(std::ifstream(Grafica::getPath(path)));
        m_window.preloadJSON(jf);
        addTransition(1, 0, Vector(0, -8), Vector(0, 3));
        /*
        auto objects = jf["transitions"].get<std::map<std::string, std::string>>();
        for (auto& it : objects) {
            addTransition(1, 0, Vector(0, -8), Vector(0, 3));
        }
        */
    }

    void Scene::clear()
    {
        m_window.clear();
        m_input.clear();
    }

    void Scene::addTransition(int fromScene, int toScene, const Vector2D& fromTile, const Vector2D& toTile)
    {
        m_transitions[fromScene] = std::pair<int, std::pair<Vector2D, Vector2D>>(
                toScene,
                std::pair<Vector2D, Vector2D>(fromTile, toTile)
        );
    }
}
