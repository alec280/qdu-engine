#pragma once
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../game_object/visual_component.hpp"
#include "../grafica/easy_shaders.h"
#include "../grafica/simple_eigen.h"
#include "../input/input.hpp"

namespace QDUEngine
{
    class Window {
        friend class Scene;
    public:
        bool shouldClose();
    private:
        void start(char *name, Vector2D& window_size, Input& input);
        void update(std::vector<std::shared_ptr<VisualComponent>>& visualComponents);
        Grafica::ModelViewProjectionShaderProgram* m_pipeline{};
        Grafica::Matrix4f* m_projection{};
        GLFWwindow* m_window{};
        VisualComponent getCube();
        void end();
    };
}