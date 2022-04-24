#include "application.hpp"

namespace QDUEngine
{
    void Application::run(const char *name, const Vector2D& windowSize, Scene& scene)
    {
        m_scene = &scene;
        m_scene->start(name, windowSize, m_keyBindings);
    }

    void Application::bindKey(const char* key, const char* action)
    {
        m_keyBindings.insert(std::pair<const char*,const char*>(key, action));
    }
}