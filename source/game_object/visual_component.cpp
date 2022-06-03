#include "visual_component.hpp"

namespace gr = Grafica;

namespace QDUEngine
{
    void VisualComponent::setGraphNodePtr(const gr::SceneGraphNodePtr& graphNodePtr)
    {
        m_graphNodePtr = graphNodePtr;
    }

    VisualComponent::VisualComponent(gr::SceneGraphNodePtr& graphNodePtr) :
        m_graphNodePtr(graphNodePtr),
        m_position{0,0}
    {}

    VisualComponent::VisualComponent() : m_position() {}

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

    void VisualComponent::clear()
    {
        m_graphNodePtr->clear();
        m_position = {};
    }

    void VisualComponent::setSource(std::string& path)
    {
        m_source = path;
    }

    nlohmann::json VisualComponent::getData()
    {
        return {
            {"name", m_name},
            {"source", m_source},
            {"posX", m_position.x},
            {"posY", m_position.y}
        };
    }

    void VisualComponent::setName(std::string& name)
    {
        m_name = name;
    }

    std::string& VisualComponent::getName()
    {
        return m_name;
    }

    std::string& VisualComponent::getSource()
    {
        return m_source;
    }
}