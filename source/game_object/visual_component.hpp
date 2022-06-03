#pragma once
#include <memory>
#include "../core/vector_2d.hpp"
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
        std::pair<std::string, std::string> getData();
        Grafica::SceneGraphNodePtr getGraphNodePtr();
        std::string& getName();
        Vector2D getPosition();
        std::string& getSource();
        void move(const Vector2D& by);
        void setName(std::string& name);
        void setGraphNodePtr(const Grafica::SceneGraphNodePtr& graphNodePtr);
        void setSource(std::string& path);
    private:
        Grafica::SceneGraphNodePtr m_graphNodePtr;
        std::string m_name;
        Vector2D m_position;
        std::string m_source;
    };
}