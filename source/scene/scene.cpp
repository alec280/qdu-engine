#include "scene.hpp"

namespace QDUEngine
{
    void Scene::addGameObject(GameObject& gameObject)
    {
        m_input.m_inputComponents.push_back(gameObject.getInputComponent());
        addVisualComponent(gameObject);
    }

    void Scene::addVisualComponent(GameObject& gameObject)
    {
        m_window.addVisualComponent(gameObject.getVisualComponent());
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
        if (m_mainObject == nullptr) {
            return;
        }
        std::string go;
        Vector2D at{0, 0};
        auto pos = m_mainObject->getVisualComponent()->getPosition();
        for (auto& transition : m_transitions) {
            if (pos == transition.second.first) {
                go = transition.first;
                at = transition.second.second;
                break;
            }
        }
        if (!go.empty()) {
            fromJSON(go.c_str());
            m_mainObject->getVisualComponent()->move(at);
        }
    }

    void Scene::end()
    {
        m_window.clear();
        m_window.end();
        if (m_tempDir != nullptr) {
            std::filesystem::remove_all(Grafica::getPath(m_tempDir));
        }
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
        return getTexturedCube(texturePath, "");
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
        if (!m_name.empty()) {
            saveJSON();
        }
        clear();
        auto fullPath = Grafica::getPath(path);
        m_name = fullPath.filename().string();
        std::string fileName = "/" + m_name;
        auto tempPath = Grafica::getPath(m_tempDir + fileName);
        nlohmann::json jf;
        if (std::filesystem::exists(tempPath)) {
            jf = nlohmann::json::parse(std::ifstream(tempPath));
        } else {
            jf = nlohmann::json::parse(std::ifstream(fullPath));
        }
        auto map = jf["map"].get<std::map<std::string, std::string>>();
        m_window.fromMap(map);
    }

    void Scene::preloadJSON(const char* path)
    {
        nlohmann::json jf = nlohmann::json::parse(std::ifstream(Grafica::getPath(path)));
        auto objects = jf["objects"].get<std::map<std::string, std::string>>();
        m_window.preload(objects);
        auto transitions = jf["transitions"].get<std::map<std::string, std::map<std::string, std::string>>>();
        for (auto& it : transitions) {
            addTransition(it.second["target"], Vector(it.first), Vector(it.second["at"]));
        }
    }

    void Scene::clear()
    {
        m_window.clear();
        if (m_mainObject != nullptr) {
            m_window.m_visualComponents.push_back(m_mainObject->getVisualComponent());
        }
        m_input.clear();
        if (m_mainObject != nullptr) {
            m_input.m_inputComponents.push_back(m_mainObject->getInputComponent());
        }
    }

    void Scene::addTransition(std::string& toScene, const Vector2D& fromTile, const Vector2D& toTile)
    {
        m_transitions[toScene] = std::pair<Vector2D, Vector2D>(fromTile, toTile);
    }

    void Scene::addMainObject(GameObject& gameObject)
    {
        auto mainInput = gameObject.getInputComponent();
        auto mainVisual = gameObject.getVisualComponent();
        mainInput->setMain(true);
        mainVisual->setMain(true);
        m_input.m_inputComponents.push_back(mainInput);
        m_window.m_visualComponents.push_back(mainVisual);
        m_mainObject = std::make_shared<GameObject>(gameObject);
    }

    SceneData Scene::getData() {
        std::map<std::string, std::string> map{};
        std::map<std::string, std::string> objects{};
        for (auto& component : m_window.m_visualComponents) {
            if (component->isMain()) {
                continue;
            }
            auto jsonData = component->getJSON();
            map[component->getPosition().toString()] = jsonData.first;
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

    void Scene::saveJSON()
    {
        if (m_tempDir == nullptr) {
            return;
        }
        auto data = getData();
        nlohmann::json jf{};
        jf["objects"] = data.objects;
        jf["map"] = data.map;
        jf["transitions"] = data.transitions;
        std::string fileName = "/" + m_name;
        std::ofstream file;
        std::filesystem::create_directories(Grafica::getPath(m_tempDir));
        auto path = Grafica::getPath(m_tempDir + fileName);
        file.open(path);
        file << jf;
        file.close();
    }

    std::shared_ptr<VisualComponent> Scene::getTexturedCube(const char* texturePath, const char* name)
    {
        return m_window.getTexturedCube(texturePath, name);
    }
}
