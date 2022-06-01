#pragma once
#include "attribute_component.hpp"
#include "visual_component.hpp"
#include "input_component.hpp"

namespace QDUEngine
{
    class Map;
    class GameObject {
    public:
        GameObject(AttributeComponent* attribute, std::shared_ptr<QDUEngine::VisualComponent>& visual);
        GameObject(AttributeComponent* attribute, std::shared_ptr<QDUEngine::VisualComponent>& visual, std::shared_ptr<QDUEngine::InputComponent>& input);
        GameObject(std::shared_ptr<QDUEngine::VisualComponent>& visual, std::shared_ptr<QDUEngine::InputComponent>& input);
        void end();
        std::shared_ptr<VisualComponent> getVisualComponent();
        std::shared_ptr<InputComponent> getInputComponent();
    private:
        AttributeComponent* m_attribute;
        std::shared_ptr<QDUEngine::VisualComponent> m_visual;
        std::shared_ptr<QDUEngine::InputComponent> m_input;
    };
}