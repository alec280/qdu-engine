#pragma once
#include <vector>
#include <fstream>
#include <glm/vec2.hpp>
#include "../debug/debug_manager.hpp"
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
        struct PerspectiveData {
            Vector2D size;
            float fovy;
            float aspect;
            float near;
            float far;
        };
        struct LoadedComponent {
            std::string texture;
            std::string object;
            std::shared_ptr<Grafica::SceneGraphNode> graphPtr;
        };
        struct MeshData {
            Grafica::Shape shape;
            std::shared_ptr<Grafica::SceneGraphNode> graphPtr;
        };
        void end();
        std::shared_ptr<Grafica::SceneGraphNode> getCubePtr(const char* texturePath);
        std::shared_ptr<VisualComponent> getMesh(const char* objPath, const char* texturePath);
        NavigationMesh getNavigationMesh(const char* objPath, const char* texturePath);
        NavigationMesh getNavigationMesh(nlohmann::json& data);
        MeshData getMeshData(const char* objPath, const char* texturePath);
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
        void update(Scene* scene, bool debug, Vector3D debugCameraPos);
        Vector3D screenToWorld(Vector2D& screenPos, Vector3D& plane, float depth);
        Vector3D m_cameraPos{0, 10, -12};
        Grafica::PhongTextureShaderProgram* m_pipeline{};
        std::vector<LoadedComponent> m_loadedComponents{};
        PerspectiveData m_perspective{};
        Grafica::Matrix4f* m_projection{};
        GLFWwindow* m_window{};
        std::shared_ptr<VisualComponent> getTexturedCube(const char* texturePath);
    };
}