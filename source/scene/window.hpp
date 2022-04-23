#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../core/vector_2d.hpp"
#include "../grafica/easy_shaders.h"
#include "../grafica/simple_eigen.h"
#include "../grafica/transformations.h"
#include "../input/input.hpp"

namespace QDUEngine
{
    class Window {
        friend class Scene;
    public:
        bool shouldClose();
    private:
        void start(char *name, Vector2D& window_size, Input& input);
        void update();
        Grafica::ModelViewProjectionShaderProgram* m_pipeline{};
        Grafica::Matrix4f* m_projection{};
        GLFWwindow* m_window{};
        void end();
    };
}