#pragma once
#include <memory>
#include <nlohmann/json.hpp>
#include "../core/vector_2d.hpp"
#include "../grafica/shape.h"
#include "../grafica/basic_shapes.h"
#include "../grafica/performance_monitor.h"
#include "../grafica/transformations.h"
#include "../grafica/scene_graph.h"

namespace QDUEngine
{
    struct VisualData {
        std::string name;
        Vector2D pos;

    };

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
        std::string& getSource();
        void move(const Vector2D& by);
        void setGraphNodePtr(const Grafica::SceneGraphNodePtr& graphNodePtr);
        void setSource(std::string& path);
    private:
        Grafica::SceneGraphNodePtr m_graphNodePtr;
        Vector2D m_position;
        std::string m_source;
    };
}