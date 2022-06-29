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

    void VisualComponent::clear()
    {
        m_graphNodePtr->clear();
        m_position = {};
    }

    void VisualComponent::loadData(nlohmann::json& data)
    {
        move(Vector(data["posX"].get<float>(), data["posY"].get<float>()));
        scale(Vector3(data.value("scaleX", 1.f),
                                 data.value("scaleY", 1.f),
                                 data.value("scaleZ", 1.f)));
        setDebugOnly(data.value("debugOnly", false));
    }

    void VisualComponent::move(const Vector2D& by)
    {
        m_position += by;
    }

    nlohmann::json VisualComponent::getData()
    {
        return {
                {"debugOnly", m_debugOnly},
                {"obj", m_obj},
                {"posX", m_position.x},
                {"posY", m_position.y},
                {"scaleX", m_scale.x},
                {"scaleY", m_scale.y},
                {"scaleZ", m_scale.z},
                {"source", m_source}
        };
    }

    gr::SceneGraphNodePtr VisualComponent::getGraphNodePtr()
    {
        return m_graphNodePtr;
    }

    Vector2D VisualComponent::getPosition()
    {
        return m_position;
    }

    Vector3D VisualComponent::getScale()
    {
        return m_scale;
    }

    std::string& VisualComponent::getSource()
    {
        return m_source;
    }

    bool VisualComponent::isDebugOnly()
    {
        return m_debugOnly;
    }

    void VisualComponent::scale(const Vector3D& to)
    {
        m_scale = to;
    }

    void VisualComponent::setDebugOnly(bool value)
    {
        m_debugOnly = value;
    }

    void VisualComponent::setObj(std::string& path)
    {
        m_obj = path;
    }

    void VisualComponent::setSource(std::string& path)
    {
        m_source = path;
    }
}