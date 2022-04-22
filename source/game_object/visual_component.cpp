#include "visual_component.hpp"

namespace gr = Grafica;
namespace tr = Grafica::Transformations;

namespace QDUEngine
{
    void VisualComponent::SetGraphNodePtr(const gr::SceneGraphNodePtr& graphNodePtr)
    {
        m_graphNodePtr = graphNodePtr;
    }

    VisualComponent::VisualComponent(gr::SceneGraphNodePtr graphNodePtr) :
    m_graphNodePtr(std::move(graphNodePtr)),
    m_position()
    {}

    VisualComponent::VisualComponent() : m_graphNodePtr(), m_position() {}

    Grafica::SceneGraphNodePtr VisualComponent::GetGraphNodePtr()
    {
        return m_graphNodePtr;
    }

    Vector2D& VisualComponent::getPosition()
    {
        return m_position;
    }

    void VisualComponent::move(const Vector2D& by)
    {
        m_position += by;
    }
}