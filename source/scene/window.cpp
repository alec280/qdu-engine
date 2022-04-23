#include "window.hpp"

namespace QDUEngine
{
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    void Window::start(char *name, QDUEngine::Vector2D& window_size)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        GLFWwindow* window = glfwCreateWindow((int)window_size.x, (int)window_size.y, name, nullptr, nullptr);
        m_window = window;

        if (window == nullptr) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(window);
        glfwSetKeyCallback(window, QDUEngine::key_callback);

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return;
        }

        auto* pipeline = new Grafica::ModelViewProjectionShaderProgram;
        m_pipeline = pipeline;
        glUseProgram(m_pipeline->shaderProgram);
        glClearColor(0, 0, 0, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        auto* projection = new Grafica::Matrix4f;
        m_projection = projection;
        Grafica::Matrix4f projection_value;
        projection_value = Grafica::Transformations::perspective(45, window_size.x/window_size.y, 0.1, 100);
        projection = &projection_value;

        while (!glfwWindowShouldClose(window)) {
            update();
        }
    }

    void Window::update()
    {
        if (!shouldClose) {
            glfwPollEvents();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUseProgram(m_pipeline->shaderProgram);
            glfwSwapBuffers(m_window);
        } else {
            glfwTerminate();
        }
    }
}

