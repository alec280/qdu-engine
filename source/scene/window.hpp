#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../core/vector_2d.hpp"
#include "../grafica/easy_shaders.h"
#include "../grafica/simple_eigen.h"
#include "../grafica/transformations.h"

namespace QDUEngine
{
    class Window {
        friend class Scene;
        void keyPressed(GLFWwindow* window);
    private:
        void start(char *name, QDUEngine::Vector2D& window_size);
        void update();
        Grafica::ModelViewProjectionShaderProgram* m_pipeline{};
        Grafica::Matrix4f* m_projection{};
        GLFWwindow* m_window{};
        bool m_shouldClose = false;
        void end();
    };
}