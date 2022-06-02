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
        std::string go;
        Vector2D at{0, 0};
        auto pos = main->getVisualComponent()->getPosition();
        for (auto& transition : m_scene.m_transitions) {
            if (pos == transition.second.first) {
                go = transition.first;
                at = transition.second.second;
                break;
            }
        }
        if (!go.empty()) {
            loadSceneFrom(go.c_str());
            main->getVisualComponent()->move(at);
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
        nlohmann::json jf;
        if (std::filesystem::exists(tempPath)) {
            jf = nlohmann::json::parse(std::ifstream(tempPath));
        } else {
            jf = nlohmann::json::parse(std::ifstream(fullPath));
        }
        auto map = jf["map"].get<std::map<std::string, std::string>>();
        for (auto& it : map) {
            auto cube = m_window.getTexturedCube(path, it.second.c_str());
            cube->move(Vector(it.first));
            auto object = GameObject(nullptr, cube);
            newScene.addGameObject(object);
        }
        auto transitions = jf["transitions"].get<std::map<std::string, std::map<std::string, std::string>>>();
        for (auto& it : transitions) {
            newScene.addTransition(it.second["target"], Vector(it.first), Vector(it.second["at"]));
        }
        log("Scene loaded from file.");
        return newScene;
    }

    std::shared_ptr<VisualComponent> Application::getTexturedCube(const char* texturePath)
    {
        return m_window.getTexturedCube(texturePath, "");
    }

    std::shared_ptr<VisualComponent> Application::getTexturedCube(const char* texturePath, const char* name)
    {
        return m_window.getTexturedCube(texturePath, name);
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

    void Application::preloadJSON(const char* path)
    {
        nlohmann::json jf = nlohmann::json::parse(std::ifstream(Grafica::getPath(path)));
        auto objects = jf["objects"].get<std::map<std::string, std::string>>();
        m_window.preload(objects);
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

    void Application::run(const char* name, float x, float y)
    {
        run(name, Vector(x, y));
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
        nlohmann::json jf{};
        jf["objects"] = data.objects;
        jf["map"] = data.map;
        jf["transitions"] = data.transitions;
        std::string fileName = "/" + m_scene.m_name;
        std::ofstream file;
        std::filesystem::create_directories(Grafica::getPath(m_tempDir));
        auto path = Grafica::getPath(m_tempDir + fileName);
        file.open(path);
        file << jf;
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