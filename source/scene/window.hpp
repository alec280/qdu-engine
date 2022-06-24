#pragma once
#include <vector>
#include <fstream>
#include <glm/vec2.hpp>
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
        struct LoadedComponent {
            std::string texture;
            std::string object;
            std::shared_ptr<Grafica::SceneGraphNode> graphPtr;
        };
        void end();
        std::shared_ptr<Grafica::SceneGraphNode> getCubePtr(const char* texturePath);
        std::shared_ptr<VisualComponent> getMesh(const char* objPath, const char* texturePath);
        std::shared_ptr<Grafica::SceneGraphNode> getMeshPtr(const char* objPath, const char* texturePath);
        std::shared_ptr<VisualComponent> makeVisualPtr(
                std::shared_ptr<Grafica::SceneGraphNode>& grPtr,
                std::string& source,
                const char* obj
        );
        static std::shared_ptr<VisualComponent> makeVisualPtr(
                std::shared_ptr<Grafica::SceneGraphNode>& grPtr,
                std::string& source,
                std::string& obj
        );
        void start(const char* name, const Vector2D& window_size, Input* input);
        void update(Scene* scene, bool debug);
        Vector2D screenToPos();
        Grafica::PhongTextureShaderProgram* m_pipeline{};
        std::vector<LoadedComponent> m_loadedComponents{};
        Grafica::Matrix4f* m_projection{};
        GLFWwindow* m_window{};
        std::shared_ptr<VisualComponent> getTexturedCube(const char* texturePath);
    };
}