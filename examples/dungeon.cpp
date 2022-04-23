#include <QDUEngine.hpp>

class Enemy : public QDUEngine::Character {
    /*
public:
    Enemy(const std::string& name, float health, float attack) : {};
    virtual void start(QDUEngine::Scene& scene) noexcept
    {

    }
    */
};

class Floor : public QDUEngine::Scene {
    /*
public:
    virtual void start(QDUEngine::Scene& scene) noexcept override
    {
        scene.addGameObject(new Enemy("evil_cube", 10, 1));
    }
    virtual void update(QDUEngine::Scene& scene, float timeStep) noexcept override {}
    */
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}


void startOpenGL() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* m_window = glfwCreateWindow((int)600,(int)600, "hewwo",nullptr,nullptr);

    if (m_window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetKeyCallback(m_window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    Grafica::ModelViewProjectionShaderProgram m_pipeline;
    glUseProgram(m_pipeline.shaderProgram);
    glClearColor(0, 0, 0, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    Grafica::Matrix4f m_projection;
    m_projection = Grafica::Transformations::perspective(45, 600.0/600.0, 0.1, 100);

    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(m_pipeline.shaderProgram);
        glfwSwapBuffers(m_window);
    }

    glfwTerminate();
}

class Test {
public:
    Test(char* name, QDUEngine::Vector2D& windowSize) : m_name(name), m_window_size(windowSize) {}
    char* m_name;
    QDUEngine::Vector2D& m_window_size;
};

int main()
{
    char name[] = "Dungeon game";
    QDUEngine::Scene floor1;
    system("pause");
    QDUEngine::Vector2D window_size = QDUEngine::Vector(600, 600);
    Test dungeon(name, window_size);
    //QDUEngine::Application dungeon(name, QDUEngine::Vector(600, 600));
    startOpenGL();
}
