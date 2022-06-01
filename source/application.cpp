#include "application.hpp"

namespace QDUEngine
{
    Application::Application(Scene* scene) : m_scene(scene) {}

    void Application::bindCursorButton(Input::CursorButton cursorButton, const char* action)
    {
        m_scene->bindCursorButton(cursorButton, action);
    }

    void Application::bindKey(const char* key, const char* action)
    {
        m_scene->bindKey(key, action);
    }

    void Application::bindJoystick(const char* key, const char* action)
    {
        m_scene->bindJoystick(key, action);
    }

    void Application::preloadJSON(const char* path)
    {
        m_scene->preloadJSON(path);
    }

    void Application::run(const char* name, const Vector2D& windowSize)
    {
        m_scene->start(name, windowSize);
        userStart();
        while (!m_shouldClose && !m_scene->m_window.shouldClose()) {
            m_scene->update(0);
        }
        m_scene->end();
    }

    void Application::run(const char* name, float x, float y)
    {
        run(name, Vector(x, y));
    }

    void Application::setTempDir(const char* path)
    {
        m_tempDir = const_cast<char*>(path);
        m_scene->m_tempDir = m_tempDir;
        if (m_tempDir != nullptr) {
            std::filesystem::remove_all(Grafica::getPath(m_tempDir));
        }
    }

    void Application::swapScenes(Scene* from, Scene* to)
    {
        to->m_window = from->m_window;
        m_scene = to;
    }

    void Application::loadSceneFrom(const char* path)
    {

    }
}