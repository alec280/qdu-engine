#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <grafica/shape.h>
#include <grafica/basic_shapes.h>
#include <grafica/performance_monitor.h>
#include <grafica/easy_shaders.h>
#include <grafica/transformations.h>
#include <grafica/scene_graph.h>

namespace gr = Grafica;
namespace tr = Grafica::Transformations;

struct Controller
{
    float cameraX = 0;
    float cameraY = 10;
    float playerPosX = 0;
    float playerPosY = 0;
} controller;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        controller.cameraY -= 1;
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        controller.cameraY += 1;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        controller.cameraX += 1;
    } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        controller.cameraX -= 1;
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        controller.playerPosY -= 1;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        controller.playerPosY += 1;
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        controller.playerPosX += 1;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        controller.playerPosX -= 1;
    }
}

template <typename PipelineT>
gr::SceneGraphNodePtr createCar(const PipelineT& pipeline, gr::Coord r, gr::Coord g, gr::Coord b)
{
    // only gpu shapes created
    auto gpuChasisPtr = std::make_shared<gr::GPUShape>(gr::toGPUShape(pipeline, gr::createColorCube(r,g,b)));
    auto gpuWeelPtr = std::make_shared<gr::GPUShape>(gr::toGPUShape(pipeline, gr::createColorCube(0,0,0)));

    // The wheel object
    auto wheelPtr = std::make_shared<gr::SceneGraphNode>("wheel", tr::scale(0.2, 0.8, 0.2), gpuWeelPtr);

    // A node to control wheel rotations
    auto wheelRotationPtr = std::make_shared<gr::SceneGraphNode>("wheelRotation");
    wheelRotationPtr->childs.push_back(wheelPtr);

    // creating wheels
    auto frontWheelPtr = std::make_shared<gr::SceneGraphNode>("frontWheel", tr::translate(0.3,0,-0.3));
    frontWheelPtr->childs.push_back(wheelRotationPtr);

    auto backWheelPtr = std::make_shared<gr::SceneGraphNode>("backWheel", tr::translate(-0.3,0,-0.3));
    backWheelPtr->childs.push_back(wheelRotationPtr);

    // Creating the chasis of the car
    auto chasisPtr = std::make_shared<gr::SceneGraphNode>("chasis", tr::scale(1,0.7,0.5), gpuChasisPtr);

    auto carPtr = std::make_shared<gr::SceneGraphNode>("car");
    carPtr->childs.push_back(chasisPtr);
    carPtr->childs.push_back(frontWheelPtr);
    carPtr->childs.push_back(backWheelPtr);

    return carPtr;
}

template <typename PipelineT>
gr::SceneGraphNodePtr createWall(const PipelineT& pipeline)
{
    auto gpuWallPtr = std::make_shared<gr::GPUShape>(gr::toGPUShape(pipeline, gr::createColorCube(0.7,0.7,0.7)));
    auto wallPtr = std::make_shared<gr::SceneGraphNode>("wall", tr::identity(), gpuWallPtr);
    return wallPtr;
}

template <typename PipelineT>
gr::SceneGraphNodePtr createPlayer(const PipelineT& pipeline)
{
    auto gpuPlayerPtr = std::make_shared<gr::GPUShape>(gr::toGPUShape(pipeline, gr::createColorCube(0,1,0)));
    auto playerPtr = std::make_shared<gr::SceneGraphNode>("player", tr::uniformScale(0.7), gpuPlayerPtr);
    return playerPtr;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    constexpr unsigned int SCR_WIDTH = 600;
    constexpr unsigned int SCR_HEIGHT = 600;

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Demo", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    gr::ModelViewProjectionShaderProgram pipeline;
    glUseProgram(pipeline.shaderProgram);

    gr::SceneGraphNodePtr sgPlayerPtr = createPlayer(pipeline);
    sgPlayerPtr->transform = tr::translate(controller.playerPosX, controller.playerPosY,0);

    gr::SceneGraphNodePtr upperWallPtr = createWall(pipeline);
    upperWallPtr->transform = tr::scale(8, 1, 1) * tr::translate(0, -3.5, 0);
    gr::SceneGraphNodePtr leftWallPtr = createWall(pipeline);
    leftWallPtr->transform = tr::scale(1, 8, 1) * tr::translate(-3.5, 0, 0);
    gr::SceneGraphNodePtr rightWallPtr = createWall(pipeline);
    rightWallPtr->transform = tr::scale(1, 8, 1) * tr::translate(3.5, 0, 0);
    gr::SceneGraphNodePtr bottomWallRightPtr = createWall(pipeline);
    bottomWallRightPtr->transform = tr::scale(2.5, 1, 1) * tr::translate(1, 3.5, 0);
    gr::SceneGraphNodePtr bottomWallLeftPtr = createWall(pipeline);
    bottomWallLeftPtr->transform = tr::scale(2.5, 1, 1) * tr::translate(-1, 3.5, 0);

    glClearColor(0, 0, 0, 1.0f);
    glEnable(GL_DEPTH_TEST);

    float t0 = glfwGetTime(), t1, dt;
    float cameraTheta = M_PI / 4;

    gr::Matrix4f projection = tr::perspective(45, float(SCR_WIDTH)/float(SCR_HEIGHT), 0.1, 100);

    // Application loop
    while (!glfwWindowShouldClose(window))
    {
        // Using GLFW to check and process input events
        glfwPollEvents();

        // Filling or not the shapes depending on the controller state
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Getting the time difference from the previous iteration
        t1 = glfwGetTime();
        dt = t1 - t0;
        t0 = t1;

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            cameraTheta -= 2 * dt;

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            cameraTheta += 2 * dt;

        gr::Vector3f const viewPos(
                controller.cameraX,
                controller.cameraY,
                12);
        gr::Vector3f const eye(controller.cameraX,controller.cameraY - 10,0);
        gr::Vector3f const at(0,0,1);

        gr::Matrix4f view = tr::lookAt(viewPos, eye, at);

        sgPlayerPtr->transform = tr::translate(controller.playerPosX, controller.playerPosY, 0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(pipeline.shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(pipeline.shaderProgram, "view"), 1, GL_FALSE, view.data());
        glUniformMatrix4fv(glGetUniformLocation(pipeline.shaderProgram, "projection"), 1, GL_FALSE, projection.data());
        glUniformMatrix4fv(glGetUniformLocation(pipeline.shaderProgram, "model"), 1, GL_FALSE, tr::identity().data());

        // Getting the shape to display
        drawSceneGraphNode(upperWallPtr, pipeline, "model");
        drawSceneGraphNode(bottomWallLeftPtr, pipeline, "model");
        drawSceneGraphNode(bottomWallRightPtr, pipeline, "model");
        drawSceneGraphNode(leftWallPtr, pipeline, "model");
        drawSceneGraphNode(rightWallPtr, pipeline, "model");
        drawSceneGraphNode(sgPlayerPtr, pipeline, "model");

        // Once the drawing is rendered, buffers are swap so an uncomplete drawing is never seen.
        glfwSwapBuffers(window);
    }

    // freeing GPU memory
    leftWallPtr->clear();
    rightWallPtr->clear();
    bottomWallLeftPtr->clear();
    bottomWallRightPtr->clear();
    upperWallPtr->clear();
    sgPlayerPtr->clear();

    glfwTerminate();
    return 0;
}
