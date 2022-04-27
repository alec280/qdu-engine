#pragma once
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../game_object/game_object.hpp"
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
        void start(const char* name, const Vector2D& window_size, Input& input);
        void update();
        Vector2D screenToPos();
        Grafica::ModelViewProjectionShaderProgram* m_pipeline{};
        std::vector<std::shared_ptr<VisualComponent>> m_visualComponents;
        Grafica::Matrix4f* m_projection{};
        GLFWwindow* m_window{};
        std::shared_ptr<VisualComponent> getCube(float r, float g, float b);
        void end();
    };
}