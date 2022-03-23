#include "Application.hpp"

namespace QDU {
    void Application::StartUp(World& world) noexcept
    {
        UserStartUp(world);
    }
}