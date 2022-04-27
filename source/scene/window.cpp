#include "window.hpp"

namespace gr = Grafica;
namespace tr = Grafica::Transformations;

namespace QDUEngine
{
    void Window::start(const char *name, const Vector2D& window_size, Input& input)
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

        for (int jid = 0; jid <= GLFW_JOYSTICK_LAST; jid++) {
            glfwSetJoystickUserPointer(jid, &input);
        }

        auto key_callback = [](GLFWwindow* w, int key, int, int action, int)
        {
            bool shouldClose = static_cast<Input*>(glfwGetWindowUserPointer(w))->keyPressed(key, action);
            glfwSetWindowShouldClose(w, shouldClose);
        };

        glfwSetKeyCallback(window, key_callback);

        auto cursor_pos_callback = [](GLFWwindow* w, double xPos, double yPos)
        {
            static_cast<Input*>(glfwGetWindowUserPointer(w))->cursorMoved(xPos, yPos);
        };

        glfwSetCursorPosCallback(window, cursor_pos_callback);

        auto mouse_button_callback = [](GLFWwindow* w, int button, int action, int)
        {
            static_cast<Input*>(glfwGetWindowUserPointer(w))->cursorPressed(button, action);
        };

        glfwSetMouseButtonCallback(window, mouse_button_callback);

        auto joystick_callback = [](int jid, int event)
        {
            static_cast<Input*>(glfwGetJoystickUserPointer(jid))->joystickCallback(jid, event);
        };

        glfwSetJoystickCallback(joystick_callback);

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

    void Window::update()
    {
        gr::Vector3f const viewPos(0, 10, -12);
        gr::Vector3f const eye(0, 0, 0);
        gr::Vector3f const at(0, 0, -1);
        gr::Matrix4f view = tr::lookAt(viewPos, eye, at);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(m_pipeline->shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(m_pipeline->shaderProgram, "view"), 1, GL_FALSE, view.data());
        glUniformMatrix4fv(glGetUniformLocation(m_pipeline->shaderProgram, "projection"), 1, GL_FALSE, m_projection->data());
        glUniformMatrix4fv(glGetUniformLocation(m_pipeline->shaderProgram, "model"), 1, GL_FALSE, tr::identity().data());

        for (auto& visualComponent : m_visualComponents) {
            auto pos = visualComponent->getPosition();
            auto nodePtr = visualComponent->getGraphNodePtr();
            nodePtr->transform = tr::translate(pos.x, pos.y,0);
            drawSceneGraphNode(nodePtr, *m_pipeline, "model");
        }

        glfwSwapBuffers(m_window);
    }

    std::shared_ptr<VisualComponent> Window::getCube(float r, float g, float b)
    {
        auto gpuCubePtr = std::make_shared<gr::GPUShape>(gr::toGPUShape(*m_pipeline, gr::createColorCube(r, g, b)));
        auto cubePtr = std::make_shared<gr::SceneGraphNode>("cube", tr::uniformScale(0.7), gpuCubePtr);
        auto component = std::make_shared<VisualComponent>(cubePtr);
        return component;
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

