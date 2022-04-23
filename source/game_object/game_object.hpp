#pragma once
#include "attribute_component.hpp"
#include "visual_component.hpp"

namespace QDUEngine
{
    class Map;
    class GameObject {
    public:
        GameObject(AttributeComponent* attribute, VisualComponent* visual);
        void update(Map& map);
    private:
        AttributeComponent* m_attribute;
        VisualComponent* m_visual;
    };
}