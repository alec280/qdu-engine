#pragma once
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include "../grafica/easy_shaders.h"
#include "../grafica/simple_eigen.h"
#include "../input/input.hpp"

namespace QDUEngine
{
    class Window {
        friend class Application;
    public:
        bool shouldClose();
    private:
        void start(const char* name, const Vector2D& window_size, Input* input);
        void update(Scene* scene);
        void end();
        void preload(std::map<std::string, std::string>& objects);
        Vector2D screenToPos();
        Grafica::PhongTextureShaderProgram* m_pipeline{};
        std::map<std::string, std::string> m_preloadPaths;
        std::map<std::string, std::shared_ptr<VisualComponent>> m_preloadComponents;
        Grafica::Matrix4f* m_projection{};
        GLFWwindow* m_window{};
        std::shared_ptr<VisualComponent> getTexturedCube(const char* texturePath, const char* name);
        std::shared_ptr<VisualComponent> getTexturedCube(const char* texturePath);
    };
}