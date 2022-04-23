#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "scene/scene.hpp"
#include "grafica/easy_shaders.h"

namespace QDUEngine
{
    class Application {
    public:
        Application(char* name, const Vector2D& windowSize);
        //void run(Scene& scene);
    public:
        Scene* m_scene{};
        char* m_name;
        Vector2D m_window_size;
        //GLFWwindow* m_window{};
        Grafica::ModelViewProjectionShaderProgram m_pipeline;
        Grafica::Matrix4f m_projection;
    };
}