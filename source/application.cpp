#include "application.hpp"

namespace QDUEngine
{
    void Application::run(char *name, Vector2D& windowSize, Scene& scene)
    {
        Window::start(name, windowSize);
    }
}