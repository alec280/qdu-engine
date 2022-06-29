#pragma once
#include <string>
#include <unordered_map>
#include "attribute_component.hpp"
#include "audio_component.hpp"
#include "visual_component.hpp"
#include "input_component.hpp"

namespace QDUEngine
{
    class Map;
    class GameObject {
        friend class Application;
    public:
        GameObject();
        GameObject(AttributeComponent* attribute, std::shared_ptr<VisualComponent>& visual);
        GameObject(AttributeComponent* attribute, std::shared_ptr<VisualComponent>& visual, std::shared_ptr<InputComponent>& input);
        GameObject(std::shared_ptr<VisualComponent>& visual, std::shared_ptr<InputComponent>& input);
        void end();
        nlohmann::json getData();
        std::string getId();
        std::shared_ptr<AudioComponent> getAudioComponent();
        std::shared_ptr<InputComponent> getInputComponent();
        std::shared_ptr<VisualComponent> getVisualComponent();
        void setAudioComponent(std::shared_ptr<AudioComponent>& audioComponent);
        void setInputComponent(std::shared_ptr<InputComponent>& inputComponent);
        friend std::ostream& operator<<(std::ostream& ostream, const GameObject& gameObject);
    private:
        AttributeComponent* m_attribute;
        std::string m_id{};
        std::shared_ptr<AudioComponent> m_audio;
        std::shared_ptr<InputComponent> m_input;
        std::shared_ptr<VisualComponent> m_visual;
    };
}