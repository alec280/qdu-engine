#include "visual_component.hpp"

namespace gr = Grafica;
namespace tr = Grafica::Transformations;

namespace QDUEngine
{
    void VisualComponent::setGraphNodePtr(const gr::SceneGraphNodePtr& graphNodePtr)
    {
        m_graphNodePtr = graphNodePtr;
    }

    VisualComponent::VisualComponent(gr::SceneGraphNodePtr& graphNodePtr) :
    m_graphNodePtr(graphNodePtr),
    m_position{1,1}
    {}

    VisualComponent::VisualComponent() : m_graphNodePtr(), m_position() {}

    gr::SceneGraphNodePtr VisualComponent::getGraphNodePtr()
    {
        return m_graphNodePtr;
    }

    Vector2D VisualComponent::getPosition()
    {
        return m_position;
    }

    void VisualComponent::move(const Vector2D& by)
    {
        m_position += by;
    }
}