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
    class GameObject;
    class VisualComponent {
    public:
        VisualComponent();
        virtual ~VisualComponent() = default;
        //virtual void update(GameObject& obj, Map& map) = 0;
        explicit VisualComponent(Grafica::SceneGraphNodePtr& graphNodePtr);
        void setGraphNodePtr(const Grafica::SceneGraphNodePtr& graphNodePtr);
        Grafica::SceneGraphNodePtr getGraphNodePtr();
        Vector2D getPosition();
        void move(const Vector2D& by);
    private:
        Vector2D m_position;
        Grafica::SceneGraphNodePtr m_graphNodePtr;
    };
}