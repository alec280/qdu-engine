#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <grafica/shape.h>
#include <grafica/basic_shapes.h>
#include <grafica/load_shaders.h>
#include <grafica/performance_monitor.h>
#include <grafica/easy_shaders.h>
#include <grafica/gpu_shape.h>
#include <grafica/transformations.h>
#include <grafica/scene_graph.h>

namespace gr = Grafica;
namespace tr = Grafica::Transformations;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
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

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    constexpr unsigned int SCR_WIDTH = 600;
    constexpr unsigned int SCR_HEIGHT = 600;

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ex_scene_graph", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Connecting the callback function 'key_callback' to handle keyboard events
    glfwSetKeyCallback(window, key_callback);

    // Loading all OpenGL function pointers with glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    gr::ModelViewProjectionShaderProgram pipeline;
    glUseProgram(pipeline.shaderProgram);

    // Creating shapes on GPU memory
    gr::GPUShape gpuAxis = gr::toGPUShape(pipeline, gr::createAxis(7));
    gr::SceneGraphNodePtr sgRedCarPtr = createCar(pipeline, 1, 0, 0);
    gr::SceneGraphNodePtr sgBlueCarPtr = createCar(pipeline, 0, 0, 1);
    sgBlueCarPtr->transform = tr::rotationZ(-M_PI/4) * tr::translate(3.0,0,0.5);

    // Setting up the clear screen color
    glClearColor(0.85f, 0.85f, 0.85f, 1.0f);

    // As we work in 3D, we need to check which part is in front,
    // and which one is at the back enabling the depth testing
    glEnable(GL_DEPTH_TEST);

    // Computing some transformations
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
                8 * std::sin(cameraTheta),
                8 * std::cos(cameraTheta),
                4);
        gr::Vector3f const eye(0,0,0);
        gr::Vector3f const at(0,0,1);

        gr::Matrix4f view = tr::lookAt(viewPos, eye, at);

        sgRedCarPtr->transform = tr::translate(3 * std::sin( t1 ),0,0.5);
        auto redWheelRotationNodeMaybe = gr::findNode(sgRedCarPtr, "wheelRotation");

        // If the node is not found, everything is lost :(
        assert(redWheelRotationNodeMaybe.has_value());

        gr::SceneGraphNode& redWheelRotationNode = *(redWheelRotationNodeMaybe.value());

        redWheelRotationNode.transform = tr::rotationY(-10 * t1);

        // Uncomment to print the red car position on every iteration
        /*auto positionMaybe = tr::findPosition(sgRedCarPtr, "car");
        assert(positionMaybe.has_value());
        auto& position = positionMaybe.value();
        std::cout << position << std::endl;*/

        // Clearing the screen in both, color and depth
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Drawing shapes with different model transformations
        glUseProgram(pipeline.shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(pipeline.shaderProgram, "view"), 1, GL_FALSE, view.data());
        glUniformMatrix4fv(glGetUniformLocation(pipeline.shaderProgram, "projection"), 1, GL_FALSE, projection.data());
        glUniformMatrix4fv(glGetUniformLocation(pipeline.shaderProgram, "model"), 1, GL_FALSE, tr::identity().data());
        pipeline.drawCall(gpuAxis, GL_LINES);

        // Getting the shape to display
        drawSceneGraphNode(sgRedCarPtr, pipeline, "model");
        drawSceneGraphNode(sgBlueCarPtr, pipeline, "model");

        // Once the drawing is rendered, buffers are swap so an uncomplete drawing is never seen.
        glfwSwapBuffers(window);
    }

    // freeing GPU memory
    gpuAxis.clear();
    sgRedCarPtr->clear();
    sgBlueCarPtr->clear();

    glfwTerminate();
    return 0;
}
