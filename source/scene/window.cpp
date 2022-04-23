#include "window.hpp"

namespace gr = Grafica;
namespace tr = Grafica::Transformations;

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

        auto* pipeline = new gr::ModelViewProjectionShaderProgram;
        m_pipeline = pipeline;
        glUseProgram(m_pipeline->shaderProgram);
        glClearColor(0, 0, 0, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        auto* projection = new gr::Matrix4f(gr::Transformations::perspective(45, window_size.x/window_size.y, 0.1, 100));
        m_projection = projection;
    }

    void Window::update(std::vector<VisualComponent*>& visualComponents)
    {
        gr::Vector3f const viewPos(0, 10, 12);
        gr::Vector3f const eye(0, 0, 0);
        gr::Vector3f const at(0, 0, 1);
        gr::Matrix4f view = tr::lookAt(viewPos, eye, at);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(m_pipeline->shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(m_pipeline->shaderProgram, "view"), 1, GL_FALSE, view.data());
        glUniformMatrix4fv(glGetUniformLocation(m_pipeline->shaderProgram, "projection"), 1, GL_FALSE, m_projection->data());
        glUniformMatrix4fv(glGetUniformLocation(m_pipeline->shaderProgram, "model"), 1, GL_FALSE, tr::identity().data());

        for (auto component : visualComponents) {
            drawSceneGraphNode(component->getGraphNodePtr(), *m_pipeline, "model");
        }

        glfwSwapBuffers(m_window);
    }

    VisualComponent Window::getCube()
    {
        auto gpuCubePtr = std::make_shared<gr::GPUShape>(gr::toGPUShape(*m_pipeline, gr::createColorCube(1, 1, 1)));
        auto cubePtr = std::make_shared<gr::SceneGraphNode>("cube", tr::uniformScale(0.7), gpuCubePtr);
        return VisualComponent(cubePtr);
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

