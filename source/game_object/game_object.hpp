#pragma once
#include <string>
#include <unordered_map>
#include "attribute_component.hpp"
#include "visual_component.hpp"
#include "input_component.hpp"

namespace QDUEngine
{
    class Map;
    class GameObject {
        friend class Application;
    public:
        GameObject(AttributeComponent* attribute, std::shared_ptr<VisualComponent>& visual);
        GameObject(AttributeComponent* attribute, std::shared_ptr<VisualComponent>& visual, std::shared_ptr<InputComponent>& input);
        GameObject(std::shared_ptr<VisualComponent>& visual, std::shared_ptr<InputComponent>& input);
        void end();
        nlohmann::json getData();
        const std::string getId();
        std::shared_ptr<InputComponent> getInputComponent();
        std::shared_ptr<VisualComponent> getVisualComponent();
        void setInputComponent(std::shared_ptr<InputComponent>& inputComponent);
        friend std::ostream& operator<<(std::ostream& ostream, const GameObject& gameObject);
    private:
        AttributeComponent* m_attribute;
        std::string m_id{};
        std::shared_ptr<InputComponent> m_input;
        std::shared_ptr<VisualComponent> m_visual;
    };
}