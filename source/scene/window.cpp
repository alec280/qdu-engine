#include "window.hpp"

namespace gr = Grafica;
namespace tr = Grafica::Transformations;

namespace QDUEngine
{
    void Window::end()
    {
        glfwSetWindowShouldClose(m_window, 1);
        glfwTerminate();
    }

    Vector2D Window::screenToPos()
    {
        return Vector(0, 0);
    }

    std::shared_ptr<Grafica::SceneGraphNode> Window::getCubePtr(const char* texturePath)
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
        return cubePtr;
    }

    std::shared_ptr<VisualComponent> Window::getMesh(const char* objPath, const char* texturePath)
    {
        std::string source = texturePath;
        for (auto& element : m_loadedComponents) {
            if (element.texture == texturePath && element.object == objPath) {
                auto meshPtr = element.graphPtr;
                return makeVisualPtr(meshPtr, source, objPath);
            }
        }
        auto meshPtr = getMeshData(objPath, texturePath).graphPtr;
        m_loadedComponents.push_back({texturePath, objPath, meshPtr});
        return makeVisualPtr(meshPtr, source, objPath);
    }

    Window::MeshData Window::getMeshData(const char* objPath, const char* texturePath)
    {
        gr::Shape shape(8);
        std::ifstream in(gr::getPath(objPath).c_str(), std::ios::in);
        if (!in) {
            std::cerr << "[Engine] Cannot open: " << objPath << std::endl;
            return {shape, nullptr};
        }
        std::string line;
        while (std::getline(in, line)) {
            if (line.substr(0, 2) == "v ") {
                std::istringstream v(line.substr(2));
                glm::vec3 vert;
                float x, y, z;
                v >> x;
                v >> y;
                v >> z;
                shape.vertices.push_back(x);
                shape.vertices.push_back(y);
                shape.vertices.push_back(z);
                shape.vertices.push_back(0.f);
                shape.vertices.push_back(0.f);
                shape.vertices.push_back(0.f);
                shape.vertices.push_back(0.f);
                shape.vertices.push_back(1.f);
            } else if (line.substr(0,2) == "f ") {
                std::istringstream v(line.substr(2));
                glm::vec3 vert;
                unsigned int a, b, c;
                v >> a;
                v >> b;
                v >> c;
                shape.indices.push_back(--a);
                shape.indices.push_back(--b);
                shape.indices.push_back(--c);
            }
        }
        auto texturedPtr = std::make_shared<gr::GPUShape>(gr::toGPUShape(*m_pipeline, shape));
        texturedPtr->texture = gr::textureSimpleSetup(
                gr::getPath(texturePath),
                GL_CLAMP_TO_EDGE,
                GL_CLAMP_TO_EDGE,
                GL_NEAREST,
                GL_NEAREST
        );
        auto meshPtr = std::make_shared<gr::SceneGraphNode>(
                "mesh",
                tr::identity(),
                texturedPtr
        );
        return {shape, meshPtr};
    }

    NavigationMesh Window::getNavigationMesh(const char* objPath, const char* texturePath)
    {
        auto navMesh = NavigationMesh(getMesh(objPath, texturePath));
        auto shape = getMeshData(objPath, texturePath).shape;
        std::vector<float> coords{};
        bool swap = false;
        for (int i = 0; i < shape.indices.size();) {
            coords.push_back(shape.vertices[shape.indices[i] * 8]);
            coords.push_back(shape.vertices[shape.indices[i] * 8 + 1]);
            if (coords.size() == 4) {
                auto begin = Vector(coords.at(0), coords.at(1));
                auto end = Vector(coords.at(2), coords.at(3));
                navMesh.addCell(begin, end);
                coords.clear();
            }
            swap = !swap;
            if (swap) {
                i += 2;
            } else {
                i += 4;
            }
        }
        const Vector2D ORTHOGONAL_VECTORS[]{
            Vector(1, 0),
            Vector(-1, 0),
            Vector(0, 1),
            Vector(0, -1)
        };
        auto cells = navMesh.getCells();
        for (int i = 0; i < cells.size(); i++) {
            auto cell = cells[i];
            for (int j = 0; j < cells.size(); j++) {
                if (i == j) {
                    continue;
                }
                auto cellSecond = cells[j];
                for (auto& vec : ORTHOGONAL_VECTORS) {
                    if (cell.begin + vec == cellSecond.begin) {
                        navMesh.addConnection(i, j, true);
                    }
                }
            }
        }
        return navMesh;
    }

    NavigationMesh Window::getNavigationMesh(nlohmann::json& data)
    {
        auto objPath = data["obj"].get<std::string>();
        auto texturePath = data["source"].get<std::string>();
        auto navMesh = getNavigationMesh(objPath.c_str(), texturePath.c_str());
        navMesh.getVisualComponent()->loadData(data);
        return navMesh;
    }

    std::shared_ptr<VisualComponent> Window::getTexturedCube(const char* texturePath)
    {
        std::string source = texturePath;
        for (auto& element : m_loadedComponents) {
            if (element.texture == texturePath && element.object.empty()) {
                auto cubePtr = element.graphPtr;
                return makeVisualPtr(cubePtr, source, "");
            }
        }
        auto cubePtr = getCubePtr(texturePath);
        m_loadedComponents.push_back({texturePath, "", cubePtr});
        return makeVisualPtr(cubePtr, source, "");
    }

    std::shared_ptr<VisualComponent> Window::makeVisualPtr(
            std::shared_ptr<Grafica::SceneGraphNode>& grPtr,
            std::string& source,
            const char* obj
    )
    {
        auto objString = std::string(obj);
        return makeVisualPtr(grPtr, source, objString);
    }

    std::shared_ptr<VisualComponent> Window::makeVisualPtr(
            std::shared_ptr<Grafica::SceneGraphNode>& grPtr,
            std::string& source,
            std::string& obj
    )
    {
        auto graph = Grafica::SceneGraphNode("");
        graph.childs.push_back(grPtr);
        auto graphPtr = std::make_shared<Grafica::SceneGraphNode>(graph);
        auto visualComponent = VisualComponent(graphPtr);
        visualComponent.setSource(source);
        visualComponent.setObj(obj);
        auto visualPtr = std::make_shared<VisualComponent>(visualComponent);
        return visualPtr;
    }

    bool Window::shouldClose()
    {
        return glfwWindowShouldClose(m_window) != 0;
    }

    void Window::start(const char *name, const Vector2D& window_size, Input* input)
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
        glfwSetWindowUserPointer(window, input);

        for (int jid = 0; jid <= GLFW_JOYSTICK_LAST; jid++) {
            glfwSetJoystickUserPointer(jid, input);
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
    }

    void Window::update(Scene* scene, bool debug, Vector3D debugCameraPos)
    {
        gr::Vector3f const viewPos(0, 10, -12);
        gr::Vector3f const eye(0, 0, 0);
        gr::Vector3f const at(0, 0, -1);
        gr::Matrix4f view{};

        if (debug) {
            gr::Vector3f viewPosDebug(debugCameraPos.x, debugCameraPos.y, debugCameraPos.z);
            gr::Vector3f eyeDebug(debugCameraPos.x, debugCameraPos.y - 10, debugCameraPos.z + 12);
            view = tr::lookAt(viewPosDebug, eyeDebug, at);
        } else {
            view = tr::lookAt(viewPos, eye, at);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(m_pipeline->shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(m_pipeline->shaderProgram, "view"), 1, GL_FALSE, view.data());
        glUniformMatrix4fv(glGetUniformLocation(m_pipeline->shaderProgram, "projection"), 1, GL_FALSE, m_projection->data());
        glUniform3f(glGetUniformLocation(m_pipeline->shaderProgram, "viewPosition"), viewPos[0], viewPos[1], viewPos[2]);

        for (auto& object : scene->getObjects()) {
            auto visualComponent = object->getVisualComponent();
            if (visualComponent == nullptr) {
                continue;
            }
            if (visualComponent->isDebugOnly() && !debug) {
                continue;
            }
            auto pos = visualComponent->getPosition();
            auto scale = visualComponent->getScale();
            auto nodePtr = visualComponent->getGraphNodePtr();
            nodePtr->transform = tr::translate(pos.x, pos.y,0) *
                    tr::scale(scale.x, scale.y, scale.z);
            drawSceneGraphNode(nodePtr, *m_pipeline, "model");
        }
        if (debug) {
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        glfwSwapBuffers(m_window);
    }
}

