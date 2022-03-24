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
        glEnable(GL_DEPTH_TEST);
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

        glUseProgram(m_shaderProgram);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
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
