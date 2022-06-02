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
        void end();
        std::shared_ptr<Grafica::SceneGraphNode> getCubePtr(const char* texturePath);
        static std::shared_ptr<VisualComponent> makeVisualPtr(
                std::shared_ptr<Grafica::SceneGraphNode>& grPtr,
                std::string& name,
                std::string& source
        );
        void start(const char* name, const Vector2D& window_size, Input* input);
        void update(Scene* scene);
        void preload(std::map<std::string, std::string>& objects);
        Vector2D screenToPos();
        Grafica::PhongTextureShaderProgram* m_pipeline{};
        std::map<std::string, std::string> m_loadedSources;
        std::map<std::string, std::shared_ptr<Grafica::SceneGraphNode>> m_loadedComponents;
        Grafica::Matrix4f* m_projection{};
        GLFWwindow* m_window{};
        std::shared_ptr<VisualComponent> getTexturedCube(const char* texturePath, const char* name);
        std::shared_ptr<VisualComponent> getTexturedCube(const char* texturePath);
    };
}