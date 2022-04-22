#include "character.hpp"

namespace QDUEngine
{
    Character::Character(AttributeComponent *attribute, VisualComponent* visual) :
        m_attribute(attribute),
        m_visual(visual)
    {}
}

