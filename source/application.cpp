#include "application.hpp"

namespace QDUEngine
{
    Application::Application(Scene* scene) : m_scene(scene) {}

    void Application::run(const char *name, const Vector2D& windowSize)
    {
        m_scene->start(name, windowSize);
    }

    void Application::run(const char *name, float x, float y)
    {
        run(name, Vector(x, y));
    }

    void Application::bindCursorButton(const char *key, const char *action)
    {
        m_scene->bindCursorButton(key, action);
    }

    void Application::bindKey(const char* key, const char* action)
    {
        m_scene->bindKey(key, action);
    }

    void Application::bindJoystick(const char *key, const char *action)
    {
        m_scene->bindJoystick(key, action);
    }

    void Application::preloadJSON(const char *path)
    {
        m_scene->preloadJSON(path);
    }
}