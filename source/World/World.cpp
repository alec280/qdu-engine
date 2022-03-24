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
        glfwInit();
        m_application = std::move(app);
        m_application.StartUp(*this);
    }

    World::~World() {
        m_application.UserShutDown(*this);
        glfwTerminate();
    }

    void World::EndApplication() noexcept {
        m_shouldClose = true;
    }

    GLFWwindow* World::GetWindow() noexcept {
        return m_window;
    }

    void World::CreateWindow(char* name) noexcept {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        m_window = glfwCreateWindow(800, 600, name, nullptr, nullptr);
        glfwMakeContextCurrent(m_window);
        glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
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
        ProcessInput();
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_application.UserUpdate(*this, timeStep);
        glfwSwapBuffers(m_window);
        glfwPollEvents();
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
