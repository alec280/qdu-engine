#include "World.hpp"

#include <chrono>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

namespace QDU {
    World::World(Application& app) :
        m_application(app),
        m_shouldClose(false),
        m_window(nullptr)
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

    GLFWwindow* World::GetWindow() noexcept {
        return m_window;
    }

    void World::SetWindow(char* name) noexcept {
        m_window = glfwCreateWindow(800, 600, name, nullptr, nullptr);
        glfwMakeContextCurrent(m_window);
        glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
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

    void World::ProcessInput() noexcept
    {
        if (glfwGetKey(GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            EndApplication();
    }

    void World::Update(float timeStep) noexcept
    {
        m_application.UserUpdate(*this, timeStep);
    }

    unsigned int World::GetShaderProgram() const {
        return m_shaderProgram;
    }

    void World::SetShaderProgram(unsigned int shaderProgram) {
        m_shaderProgram = shaderProgram;
    }

    unsigned int World::GetVAO() const {
        return m_vao;
    }

    void World::SetVAO(unsigned int vao) {
        m_vao = vao;
    }
}
