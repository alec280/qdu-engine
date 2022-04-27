#include "application.hpp"

namespace QDUEngine
{
    void Application::run(const char *name, const Vector2D& windowSize, std::shared_ptr<QDUEngine::Scene> scene)
    {
        m_scene = scene;
        m_scene->start(name, windowSize, m_cursorBindings, m_keyBindings, m_joystickBindings);
    }

    void Application::bindCursorButton(const char *key, const char *action)
    {
        m_cursorBindings.insert(std::pair<const char*,const char*>(key, action));
    }

    void Application::bindKey(const char* key, const char* action)
    {
        m_keyBindings.insert(std::pair<const char*,const char*>(key, action));
    }

    void Application::bindJoystick(const char *key, const char *action)
    {
        m_joystickBindings.insert(std::pair<const char*,const char*>(key, action));
    }
}