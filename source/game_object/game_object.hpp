#pragma once
#include "attribute_component.hpp"
#include "visual_component.hpp"
#include "input_component.hpp"

namespace QDUEngine
{
    class Map;
    class GameObject {
    public:
        GameObject(AttributeComponent* attribute, VisualComponent* visual, InputComponent* input);
        void update(Map& map);
        void end();
        VisualComponent getVisualComponent();
        InputComponent getInputComponent();
    private:
        AttributeComponent* m_attribute;
        VisualComponent* m_visual;
        InputComponent* m_input;
    };
}