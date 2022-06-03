#include "application.hpp"

namespace QDUEngine
{
    Application::Application() = default;

    void Application::bindCursorButton(Input::CursorButton cursorButton, const char* action)
    {
        m_input.m_cursorBindings[cursorButton] = action;
        m_input.m_cursorActions[std::string(action)] = 0;
    }

    void Application::bindKey(const char* key, const char* action)
    {
        m_input.m_keyBindings[std::string(key)] = action;
        m_input.m_actions[std::string(action)] = 0;
    }

    void Application::bindJoystick(const char* key, const char* action)
    {
        m_input.m_joystickBindings[std::string(key)] = action;
        m_input.m_actions[std::string(action)] = 0;
    }

    void Application::doTransition()
    {
        auto main = m_scene.m_mainObject;
        if (main == nullptr) {
            return;
        }
        std::string toScene;
        Vector2D toPos{};
        auto pos = main->getVisualComponent()->getPosition();
        for (auto& transition : m_scene.m_transitions) {
            if (pos == transition.second.first) {
                toScene = transition.first;
                toPos = transition.second.second;
                break;
            }
        }
        if (!toScene.empty()) {
            loadSceneFrom(toScene.c_str());
            main->getVisualComponent()->move(toPos);
            m_scene.addMainObject(main);
        }
    }

    Scene Application::getSceneFrom(const char* path)
    {
        if (m_tempDir == nullptr) {
            log("Temp directory not set. Will get an empty scene.");
            return {};
        }
        log("Loading scene from file.");
        auto newScene = Scene();
        auto fullPath = Grafica::getPath(path);
        auto sceneName = fullPath.filename().string();
        auto fileName = "/" + sceneName;
        auto tempPath = Grafica::getPath(m_tempDir + fileName);
        newScene.m_name = sceneName;
        newScene.m_source = path;
        nlohmann::json data;
        if (std::filesystem::exists(tempPath)) {
            data = nlohmann::json::parse(std::ifstream(tempPath));
        } else {
            data = nlohmann::json::parse(std::ifstream(fullPath));
        }
        auto objects = data["objects"];
        for (auto it = objects.begin(); it != objects.end(); ++it) {
            auto objectData = *it;
            auto visual = objectData["visual"];
            auto cube = getTexturedCube(visual["source"].get<std::string>().c_str());
            cube->move(Vector(visual["posX"].get<float>(), visual["posY"].get<float>()));
            std::shared_ptr<InputComponent> input = nullptr;
            for (auto& element : m_input.m_loadedComponents) {
                if (element.first == objectData.value("id", "") && !element.first.empty()) {
                    input = element.second;
                    break;
                }
            }
            auto object = GameObject(nullptr, cube);
            if (input != nullptr) {
                auto objectPtr = std::make_shared<GameObject>(object);
                input->setGameObject(objectPtr);
            }
            newScene.addGameObject(object);
        }
        auto transitions = data["transitions"];
        for (auto it = transitions.begin(); it != transitions.end(); ++it) {
            auto toSceneName = it.key();
            auto transitionData = it.value();
            auto fromVector = Vector(transitionData["fromX"].get<float>(), transitionData["fromY"].get<float>());
            auto toVector = Vector(transitionData["toX"].get<float>(), transitionData["toY"].get<float>());
            newScene.addTransition(toSceneName, fromVector, toVector);
        }
        log("Scene loaded from file.");
        return newScene;
    }

    std::shared_ptr<VisualComponent> Application::getTexturedCube(const char* texturePath)
    {
        return m_window.getTexturedCube(texturePath);
    }

    void Application::loadSceneFrom(const char* path)
    {
        auto nextScene = getSceneFrom(path);
        setScene(nextScene);
    }

    void Application::log(const char* msg)
    {
        std::cout << "[Engine] " << msg << std::endl;
    }

    void Application::run(const char* name, const Vector2D& windowSize)
    {
        log("PRE-START");
        m_window.start(name, windowSize, &m_input);
        userStart();
        log("START");
        while (!m_window.shouldClose()) {
            m_input.update(&m_scene);
            m_window.update(&m_scene);
            m_scene.update();
            doTransition();
        }
        log("PRE-END");
        m_scene.end();
        if (m_tempDir != nullptr) {
            std::filesystem::remove_all(Grafica::getPath(m_tempDir));
            log("Temporary directory cleared.");
        }
        log("END");
    }

    void Application::run(const char* name, float windowSizeX, float windowSizeY)
    {
        run(name, Vector(windowSizeX, windowSizeY));
    }

    void Application::saveScene()
    {
        if (m_scene.m_name.empty()) {
            return;
        }
        if (m_tempDir == nullptr) {
            log("Can't save current scene without temporary directory.");
            return;
        }
        auto data = m_scene.getData();
        auto objects = data["objects"];
        for (auto it = objects.begin(); it != objects.end(); ++it) {
            auto objectData = *it;
            auto objectId = objectData["id"].get<std::string>();
            auto object = m_scene.getById(objectId);
            if (object != nullptr) {
                auto input = object->getInputComponent();
                if (input != nullptr) {
                    m_input.m_loadedComponents[objectId] = input;
                }
            }
        }
        std::string fileName = "/" + m_scene.m_name;
        std::ofstream file;
        std::filesystem::create_directories(Grafica::getPath(m_tempDir));
        auto path = Grafica::getPath(m_tempDir + fileName);
        file.open(path);
        file << data;
        file.close();
    }

    void Application::setGlobalInput(std::shared_ptr<InputComponent>& inputComponent)
    {
        m_input.m_globalInput = inputComponent;
    }

    void Application::setScene(Scene& scene)
    {
        log("Ending previous scene.");
        saveScene();
        m_scene.end();
        m_scene = scene;
        log("Previous scene ended.");
    }

    void Application::setTempDir(const char* path)
    {
        m_tempDir = const_cast<char*>(path);
        if (m_tempDir != nullptr) {
            std::filesystem::remove_all(Grafica::getPath(m_tempDir));
        }
    }
}