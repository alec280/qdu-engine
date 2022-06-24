#pragma once
#include <memory>
#include <nlohmann/json.hpp>
#include "../core/vector_3d.hpp"
#include "../grafica/shape.h"
#include "../grafica/basic_shapes.h"
#include "../grafica/performance_monitor.h"
#include "../grafica/transformations.h"
#include "../grafica/scene_graph.h"

namespace QDUEngine
{
    class Map;
    class VisualComponent {
    public:
        VisualComponent();
        explicit VisualComponent(Grafica::SceneGraphNodePtr& graphNodePtr);
        virtual ~VisualComponent() = default;
        void clear();
        nlohmann::json getData();
        Grafica::SceneGraphNodePtr getGraphNodePtr();
        Vector2D getPosition();
        Vector3D getScale();
        std::string& getSource();
        bool isDebugOnly();
        void move(const Vector2D& by);
        void setDebugOnly(bool value);
        void setGraphNodePtr(const Grafica::SceneGraphNodePtr& graphNodePtr);
        void setObj(std::string& path);
        void setSource(std::string& path);
        void scale(const Vector3D& to);
    private:
        bool m_debugOnly = false;
        Grafica::SceneGraphNodePtr m_graphNodePtr;
        std::string m_obj;
        Vector2D m_position;
        Vector3D m_scale = {1, 1, 1};
        std::string m_source;
    };
}