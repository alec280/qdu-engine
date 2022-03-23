#include "World.hpp"

#include <chrono>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace QDU {
    World::World(Application& app) :
        m_application(app),
        m_shouldClose(false)
    {
        m_application = std::move(app);
        m_application.StartUp(*this);
    }

    World::~World() {
        m_application.UserShutDown(*this);
    }

    void World::EndApplication() noexcept {
        m_shouldClose = true;
    }

    void World::StartMainLoop() noexcept {
        std::chrono::time_point<std::chrono::steady_clock> startTime = std::chrono::steady_clock::now();
        while (!m_shouldClose)
        {
            std::chrono::time_point<std::chrono::steady_clock> newTime = std::chrono::steady_clock::now();
            const auto frameTime = newTime - startTime;
            startTime = newTime;
            float timeStep = std::chrono::duration_cast<std::chrono::duration<float>>(frameTime).count();
            Update(timeStep);
        }
    }

    void World::Update(float timeStep) noexcept
    {
        m_application.UserUpdate(*this, timeStep);
    }
}
