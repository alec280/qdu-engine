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
        m_position{0,0}
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

    void VisualComponent::clear()
    {
        m_graphNodePtr->clear();
        m_position = {};
    }

    void VisualComponent::setSource(std::string& path)
    {
        m_source = path;
    }

    std::pair<std::string, std::string> VisualComponent::getData()
    {
        return {m_name, m_source};
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