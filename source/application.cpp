#include "application.hpp"

namespace QDUEngine
{
    void Application::run(char *name, const Vector2D& windowSize, Scene& scene)
    {
        m_scene = &scene;
        m_scene->start(name, windowSize);
    }
}