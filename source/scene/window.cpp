#include "window.hpp"

namespace QDUEngine
{
    void Window::start(char *name, Vector2D& window_size, Input& input)
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
        glfwSetWindowUserPointer(window, &input);

        auto func = [](GLFWwindow* w, int, int, int, int)
        {
            static_cast<Input*>(glfwGetWindowUserPointer(w))->keyPressed();
        };

        glfwSetKeyCallback(window, func);

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
    }

    void Window::update()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(m_pipeline->shaderProgram);
        glfwSwapBuffers(m_window);
    }

    void Window::end()
    {
        glfwSetWindowShouldClose(m_window, true);
        glfwTerminate();
    }

    bool Window::shouldClose()
    {
        return glfwWindowShouldClose(m_window);
    }
}

