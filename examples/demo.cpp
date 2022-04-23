#include <QDUEngine.hpp>
/*
namespace gr = Grafica;
namespace tr = Grafica::Transformations;

struct Controller
{
    float cameraX = 0;
    float cameraY = 10;
    float playerPosX = 0;
    float playerPosY = 0;
    float enemyPosX = 10;
    float enemyPosY = 0;
    bool playerMoved = false;
    int enemyStep = 0;
    const float enemyRoute[8][2]{{10, 0}, {11, 0}, {11, 1}, {10, 1}, {9, 1}, {9, 0}, {9, -1}, {10, -1}};
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
        controller.playerMoved = true;
        controller.playerPosY -= 1;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        controller.playerMoved = true;
        controller.playerPosY += 1;
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        controller.playerMoved = true;
        controller.playerPosX += 1;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        controller.playerMoved = true;
        controller.playerPosX -= 1;
    }
}

template <typename PipelineT>
gr::SceneGraphNodePtr createWall(const PipelineT& pipeline)
{
    auto gpuWallPtr = std::make_shared<gr::GPUShape>(gr::toGPUShape(pipeline, gr::createColorCube(0.7,0.7,0.7)));
    auto wallPtr = std::make_shared<gr::SceneGraphNode>("wall", tr::identity(), gpuWallPtr);
    return wallPtr;
}

template <typename PipelineT>
gr::SceneGraphNodePtr createRoom(const PipelineT& pipeline)
{
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

    auto roomPtr = std::make_shared<gr::SceneGraphNode>("room");
    roomPtr->childs.push_back(upperWallPtr);
    roomPtr->childs.push_back(leftWallPtr);
    roomPtr->childs.push_back(rightWallPtr);
    roomPtr->childs.push_back(bottomWallRightPtr);
    roomPtr->childs.push_back(bottomWallLeftPtr);

    return roomPtr;
}

template <typename PipelineT>
gr::SceneGraphNodePtr createChest(const PipelineT& pipeline)
{
    auto gpuBodyPtr = std::make_shared<gr::GPUShape>(gr::toGPUShape(pipeline, gr::createColorCube(0.5,0.3,0)));
    auto gpuLockPtr = std::make_shared<gr::GPUShape>(gr::toGPUShape(pipeline, gr::createColorCube(0.2,0.2,0.2)));

    auto lockPtr = std::make_shared<gr::SceneGraphNode>("lock", tr::uniformScale(0.3) * tr::translate(0, 1, 0), gpuLockPtr);
    auto bodyPtr = std::make_shared<gr::SceneGraphNode>("body", tr::uniformScale(0.7), gpuBodyPtr);
    auto chestPtr = std::make_shared<gr::SceneGraphNode>("chest");
    chestPtr->childs.push_back(bodyPtr);
    chestPtr->childs.push_back(lockPtr);

    return chestPtr;
}

template <typename PipelineT>
gr::SceneGraphNodePtr createPlayer(const PipelineT& pipeline)
{
    auto gpuPlayerPtr = std::make_shared<gr::GPUShape>(gr::toGPUShape(pipeline, gr::createColorCube(0,1,0)));
    auto playerPtr = std::make_shared<gr::SceneGraphNode>("player", tr::uniformScale(0.7), gpuPlayerPtr);
    return playerPtr;
}

template <typename PipelineT>
gr::SceneGraphNodePtr createEnemy(const PipelineT& pipeline)
{
    auto gpuPlayerPtr = std::make_shared<gr::GPUShape>(gr::toGPUShape(pipeline, gr::createColorCube(1,0,0)));
    auto playerPtr = std::make_shared<gr::SceneGraphNode>("enemy", tr::uniformScale(0.7), gpuPlayerPtr);
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

    gr::SceneGraphNodePtr playerPtr = createPlayer(pipeline);
    playerPtr->transform = tr::translate(controller.playerPosX, controller.playerPosY,0);

    gr::SceneGraphNodePtr middleRoomPtr = createRoom(pipeline);

    gr::SceneGraphNodePtr leftRoomPtr = createRoom(pipeline);
    leftRoomPtr->transform = tr::translate(-10, 0,0);

    gr::SceneGraphNodePtr rightRoomPtr = createRoom(pipeline);
    rightRoomPtr->transform = tr::translate(10, 0,0);

    gr::SceneGraphNodePtr chestPtr = createChest(pipeline);
    chestPtr->transform = tr::translate(-10, -0.5,0);

    gr::SceneGraphNodePtr enemyPtr = createEnemy(pipeline);
    enemyPtr->transform = tr::translate(
            controller.enemyRoute[controller.enemyStep][0],
            controller.enemyRoute[controller.enemyStep][1],
            0);

    glClearColor(0, 0, 0, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    float t0 = glfwGetTime(), t1, dt;
    float cameraTheta = M_PI / 4;

    gr::Matrix4f projection = tr::perspective(45, float(SCR_WIDTH)/float(SCR_HEIGHT), 0.1, 100);

    // Application loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

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

        playerPtr->transform = tr::translate(controller.playerPosX, controller.playerPosY, 0);

        if (controller.playerMoved) {
            controller.playerMoved = false;
            controller.enemyStep += 1;
            if (controller.enemyStep == 8) {
                controller.enemyStep = 0;
            }
        }

        enemyPtr->transform = tr::translate(
                controller.enemyRoute[controller.enemyStep][0],
                controller.enemyRoute[controller.enemyStep][1],
                0);

        if (controller.playerPosX == controller.enemyPosX && controller.enemyPosY == controller.playerPosY) {
            glfwSetWindowShouldClose(window, true);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(pipeline.shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(pipeline.shaderProgram, "view"), 1, GL_FALSE, view.data());
        glUniformMatrix4fv(glGetUniformLocation(pipeline.shaderProgram, "projection"), 1, GL_FALSE, projection.data());
        glUniformMatrix4fv(glGetUniformLocation(pipeline.shaderProgram, "model"), 1, GL_FALSE, tr::identity().data());

        drawSceneGraphNode(chestPtr, pipeline, "model");
        drawSceneGraphNode(leftRoomPtr, pipeline, "model");
        drawSceneGraphNode(rightRoomPtr, pipeline, "model");
        drawSceneGraphNode(middleRoomPtr, pipeline, "model");
        drawSceneGraphNode(playerPtr, pipeline, "model");
        drawSceneGraphNode(enemyPtr, pipeline, "model");
        glfwSwapBuffers(window);
    }

    chestPtr->clear();
    leftRoomPtr->clear();
    rightRoomPtr->clear();
    middleRoomPtr->clear();
    playerPtr->clear();
    enemyPtr->clear();

    glfwTerminate();
    return 0;
}
*/

int main() {
    return 0;
}
