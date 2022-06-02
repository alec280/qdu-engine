#pragma once
#include "attribute_component.hpp"
#include "visual_component.hpp"
#include "input_component.hpp"

namespace QDUEngine
{
    class Map;
    class GameObject {
    public:
        GameObject(AttributeComponent* attribute, std::shared_ptr<VisualComponent>& visual);
        GameObject(AttributeComponent* attribute, std::shared_ptr<VisualComponent>& visual, std::shared_ptr<InputComponent>& input);
        GameObject(std::shared_ptr<VisualComponent>& visual, std::shared_ptr<InputComponent>& input);
        void end();
        std::pair<std::string, std::string> getData();
        std::shared_ptr<InputComponent> getInputComponent();
        std::shared_ptr<VisualComponent> getVisualComponent();
    private:
        AttributeComponent* m_attribute;
        size_t m_id;
        std::shared_ptr<InputComponent> m_input;
        std::shared_ptr<VisualComponent> m_visual;
    };
}