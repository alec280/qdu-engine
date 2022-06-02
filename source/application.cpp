#include "application.hpp"

namespace QDUEngine
{
    //Application::Application() : m_scene(new Scene()) {}
    Application::Application(Scene* scene) : m_scene(scene) {}

    void Application::bindCursorButton(Input::CursorButton cursorButton, const char* action)
    {
        m_scene->m_input.m_cursorBindings[cursorButton] = action;
        m_scene->m_input.m_cursorActions[std::string(action)] = 0;
    }

    void Application::bindKey(const char* key, const char* action)
    {
        m_scene->m_input.m_keyBindings[std::string(key)] = action;
        m_scene->m_input.m_actions[std::string(action)] = 0;
    }

    void Application::bindJoystick(const char* key, const char* action)
    {
        m_scene->m_input.m_joystickBindings[std::string(key)] = action;
        m_scene->m_input.m_actions[std::string(action)] = 0;
    }

    void Application::preloadJSON(const char* path)
    {
        m_scene->preloadJSON(path);
    }

    void Application::run(const char* name, const Vector2D& windowSize)
    {
        log("START");
        m_scene->start(name, windowSize);
        userStart();
        while (!m_shouldClose && !m_scene->m_window.shouldClose()) {
            m_scene->update(0);
        }
        m_scene->end();
        delete m_scene;
        log("END");
    }

    void Application::run(const char* name, float x, float y)
    {
        run(name, Vector(x, y));
    }

    void Application::setGlobalInput(std::shared_ptr<InputComponent>& inputComponent)
    {
        m_scene->m_input.m_globalInput = inputComponent;
    }

    void Application::setTempDir(const char* path)
    {
        m_tempDir = const_cast<char*>(path);
        m_scene->m_tempDir = m_tempDir;
        if (m_tempDir != nullptr) {
            std::filesystem::remove_all(Grafica::getPath(m_tempDir));
        }
    }

    void Application::loadSceneFrom(const char* path)
    {
        if (m_tempDir == nullptr) {
            log("Temp directory not set.");
            return;
        }
        /*
        if (m_scene) {
            log("Finishing previous scene.");
            if (!m_scene->m_name.empty()) {
                log("Saving previous scene data.");
                m_scene->saveJSON();
                log("Previous scene data saved.");
            }
            m_scene->clear();
            //delete m_scene;
            log("Previous scene finished.");
        }
         */
        auto fullPath = Grafica::getPath(path);
        auto sceneName = fullPath.filename().string();
        log("Loading scene from file.");
        //m_nextScene = (Scene*) malloc(sizeof(Scene));
        m_scene->m_name = sceneName;
        std::string fileName = "/" + sceneName;
        auto tempPath = Grafica::getPath(m_tempDir + fileName);
        nlohmann::json jf;
        if (std::filesystem::exists(tempPath)) {
            jf = nlohmann::json::parse(std::ifstream(tempPath));
        } else {
            jf = nlohmann::json::parse(std::ifstream(fullPath));
        }
        auto map = jf["map"].get<std::map<std::string, std::string>>();
        m_scene->m_window.fromMap(map);
        log("Scene loaded from file.");
    }

    void Application::log(const char* msg)
    {
        std::cout << "[Engine] " << msg << std::endl;
    }
}