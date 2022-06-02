#pragma once
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../game_object/game_object.hpp"
#include "../grafica/easy_shaders.h"
#include "../grafica/simple_eigen.h"
#include "../input/input.hpp"

namespace QDUEngine
{
    class Window {
        friend class Application;
        friend class Scene;
    public:
        bool shouldClose();
    private:
        void start(const char* name, const Vector2D& window_size, Input* input);
        void update();
        void end();
        void clear();
        void preload(std::map<std::string, std::string>& objects);
        void fromMap(std::map<std::string, std::string>& map);
        void addVisualComponent(const std::shared_ptr<VisualComponent>& component);
        Vector2D screenToPos();
        Grafica::PhongTextureShaderProgram* m_pipeline{};
        std::vector<std::shared_ptr<VisualComponent>> m_visualComponents;
        std::map<std::string, std::string> m_preloadPaths;
        std::map<std::string, std::shared_ptr<VisualComponent>> m_preloadComponents;
        Grafica::Matrix4f* m_projection{};
        GLFWwindow* m_window{};
        std::shared_ptr<VisualComponent> getCube(float r, float g, float b);
        std::shared_ptr<VisualComponent> getTexturedCube(const char* texturePath, const char* name);
        std::shared_ptr<VisualComponent> getTexturedCube(const char* texturePath);
    };
}