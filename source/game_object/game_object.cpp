#include "game_object.hpp"

namespace QDUEngine
{
    GameObject::GameObject(AttributeComponent *attribute, VisualComponent* visual) :
        m_attribute(attribute),
        m_visual(visual)
    {}

    void GameObject::update(Map& map)
    {
        m_attribute->update(*this);
        m_visual->update(*this, map);
    }
}

