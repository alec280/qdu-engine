#include "game_object.hpp"

namespace QDUEngine
{
    GameObject::GameObject(AttributeComponent* attribute, std::shared_ptr<VisualComponent>& visual) :
            m_attribute(attribute),
            m_visual(visual),
            m_input(nullptr)
    {}

    GameObject::GameObject(AttributeComponent* attribute, std::shared_ptr<VisualComponent>& visual, std::shared_ptr<InputComponent>& input) :
            m_attribute(attribute),
            m_visual(visual),
            m_input(input)
    {}

    GameObject::GameObject(std::shared_ptr<VisualComponent>& visual, std::shared_ptr<InputComponent>& input) :
            GameObject(nullptr, visual, input)
    {}

    void GameObject::end()
    {
        if (m_visual != nullptr) {
            m_visual->getGraphNodePtr()->clear();
        }
        m_visual->move(m_visual->getPosition() * -1);
    }

    std::pair<std::string, std::string> GameObject::getData()
    {
        return m_visual->getData();
    }


    std::shared_ptr<InputComponent> GameObject::getInputComponent()
    {
        return m_input;
    }

    std::shared_ptr<VisualComponent> GameObject::getVisualComponent()
    {
        return m_visual;
    }
}

