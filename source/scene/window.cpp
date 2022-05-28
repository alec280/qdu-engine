#include "window.hpp"
#include "../grafica/root_directory.h"

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

        auto* pipeline = new gr::PhongTextureShaderProgram;
        m_pipeline = pipeline;
        glUseProgram(m_pipeline->shaderProgram);
        glClearColor(0, 0, 0, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glUniform3f(glGetUniformLocation(m_pipeline->shaderProgram, "La"), 1.0, 1.0, 1.0);
        glUniform3f(glGetUniformLocation(m_pipeline->shaderProgram, "Ld"), 1.0, 1.0, 1.0);
        glUniform3f(glGetUniformLocation(m_pipeline->shaderProgram, "Ls"), 1.0, 1.0, 1.0);

        glUniform3f(glGetUniformLocation(m_pipeline->shaderProgram, "Ka"), 0.4, 0.4, 0.4);
        glUniform3f(glGetUniformLocation(m_pipeline->shaderProgram, "Kd"), 0.9, 0.9, 0.9);
        glUniform3f(glGetUniformLocation(m_pipeline->shaderProgram, "Ks"), 1.0, 1.0, 1.0);

        glUniform3f(glGetUniformLocation(m_pipeline->shaderProgram, "lightPosition"), 5, 5, 5);
        glUniform1ui(glGetUniformLocation(m_pipeline->shaderProgram, "shininess"), 100);

        glUniform1f(glGetUniformLocation(m_pipeline->shaderProgram, "constantAttenuation"), 0.0001);
        glUniform1f(glGetUniformLocation(m_pipeline->shaderProgram, "linearAttenuation"), 0.03);
        glUniform1f(glGetUniformLocation(m_pipeline->shaderProgram, "quadraticAttenuation"), 0.01);

        auto* projection = new gr::Matrix4f(gr::Transformations::perspective(45, window_size.x/window_size.y, 0.1, 100));
        m_projection = projection;

        for (auto& it : m_preloadPaths) {
            std::shared_ptr<VisualComponent> cube = getTexturedCube(it.second.c_str());
            m_preloadComponents[it.first] = cube;
        }
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
        glUniform3f(glGetUniformLocation(m_pipeline->shaderProgram, "viewPosition"), viewPos[0], viewPos[1], viewPos[2]);

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

    std::shared_ptr<VisualComponent> Window::getTexturedCube(const char* texturePath, const char* name)
    {
        gr::Shape shape(8);
        shape.vertices = {
                //   positions         tex coords   normals
                // Z+: number 1
                -0.5f, -0.5f,  0.5f, 0.0f, 1.0f/3,  0.0f, 0.0f, 1.0f,
                0.5f, -0.5f,  0.5f, 0.5f, 1.0f/3,  0.0f, 0.0f, 1.0f,
                0.5f,  0.5f,  0.5f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f,
                -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,

                // Z-: number 6
                -0.5f, -0.5f, -0.5f, 0.5f, 1.0f,      0.0f, 0.0f, -1.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 1.0f,      0.0f, 0.0f, -1.0f,
                0.5f,  0.5f, -0.5f, 1.0f, 2.0f/3,    0.0f, 0.0f, -1.0f,
                -0.5f,  0.5f, -0.5f, 0.5f, 2.0f/3,    0.0f, 0.0f, -1.0f,

                // X+: number 5
                0.5f, -0.5f, -0.5f, 0.0f,   1.0f,   1.0f, 0.0f, 0.0f,
                0.5f,  0.5f, -0.5f, 0.5f,   1.0f,   1.0f, 0.0f, 0.0f,
                0.5f,  0.5f,  0.5f, 0.5f, 2.0f/3,   1.0f, 0.0f, 0.0f,
                0.5f, -0.5f,  0.5f, 0.0f, 2.0f/3,   1.0f, 0.0f, 0.0f,

                // X-: number 2
                -0.5f, -0.5f, -0.5f, 0.5f, 1.0f/3,   -1.0f, 0.0f, 0.0f,
                -0.5f,  0.5f, -0.5f, 1.0f, 1.0f/3,   -1.0f, 0.0f, 0.0f,
                -0.5f,  0.5f,  0.5f, 1.0f,   0.0f,   -1.0f, 0.0f, 0.0f,
                -0.5f, -0.5f,  0.5f, 0.5f,   0.0f,   -1.0f, 0.0f, 0.0f,

                // Y+: number 4
                -0.5f,  0.5f, -0.5f, 0.5f, 2.0f/3,   0.0f, 1.0f, 0.0f,
                0.5f,  0.5f, -0.5f, 1.0f, 2.0f/3,   0.0f, 1.0f, 0.0f,
                0.5f,  0.5f,  0.5f, 1.0f, 1.0f/3,   0.0f, 1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f, 0.5f, 1.0f/3,   0.0f, 1.0f, 0.0f,

                // Y-: number 3
                -0.5f, -0.5f, -0.5f, 0.0f, 2.0f/3,   0.0f, -1.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 0.5f, 2.0f/3,   0.0f, -1.0f, 0.0f,
                0.5f, -0.5f,  0.5f, 0.5f, 1.0f/3,   0.0f, -1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f, 0.0f, 1.0f/3,   0.0f, -1.0f, 0.0f
        };
        // One face of the cube per row
        shape.indices = {
                0, 1, 2, 2, 3, 0, // Z+
                7, 6, 5, 5, 4, 7, // Z-
                8, 9,10,10,11, 8, // X+
                15,14,13,13,12,15, // X-
                19,18,17,17,16,19, // Y+
                20,21,22,22,23,20 // Y-
        };
        auto texturedPtr = std::make_shared<gr::GPUShape>(gr::toGPUShape(*m_pipeline, shape));
        texturedPtr->texture = gr::textureSimpleSetup(
                gr::getPath(texturePath),
                GL_CLAMP_TO_EDGE,
                GL_CLAMP_TO_EDGE,
                GL_NEAREST,
                GL_NEAREST
        );
        auto cubePtr = std::make_shared<gr::SceneGraphNode>(
                "textured",
                tr::identity(),
                texturedPtr
        );
        auto visualComponent = std::make_shared<VisualComponent>(cubePtr);
        std::string source = texturePath;
        std::string stringName = name;
        visualComponent->setSource(source);
        visualComponent->setName(stringName);
        return visualComponent;
    }

    std::shared_ptr<VisualComponent> Window::getTexturedCube(const char* texturePath)
    {
        return getTexturedCube(texturePath, "");
    }

    Vector2D Window::screenToPos()
    {
        return Vector(0, 0);
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

    void Window::clear()
    {
        for (auto& visualComponent : m_visualComponents) {
            visualComponent->clear();
        }
        m_visualComponents.clear();
    }

    void Window::preload(std::map<std::string, std::string>& objects)
    {
        for (auto& it : objects) {
            m_preloadPaths[it.first] = it.second;
        }
    }

    void Window::fromMap(std::map<std::string, std::string>& map)
    {
        for (auto& it : map) {
            auto graph = Grafica::SceneGraphNode(it.first);
            graph.childs.push_back(m_preloadComponents[it.second]->getGraphNodePtr());
            auto tmp = std::make_shared<Grafica::SceneGraphNode>(graph);
            auto graphComponent = std::make_shared<VisualComponent>(tmp);
            //auto object = GameObject(nullptr, cube);
            graphComponent->move(QDUEngine::Vector(it.first));
            graphComponent->setName(it.second);
            graphComponent->setSource(m_preloadPaths[it.second]);
            //addGameObject(object);
            m_visualComponents.push_back(graphComponent);
        }
    }
}

