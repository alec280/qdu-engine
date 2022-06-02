#include "game_object.hpp"

namespace QDUEngine
{
    GameObject::GameObject(AttributeComponent* attribute, std::shared_ptr<QDUEngine::VisualComponent>& visual) :
            m_attribute(attribute),
            m_visual(visual),
            m_input(nullptr)
    {}

    GameObject::GameObject(AttributeComponent* attribute, std::shared_ptr<QDUEngine::VisualComponent>& visual, std::shared_ptr<QDUEngine::InputComponent>& input) :
        m_attribute(attribute),
        m_visual(visual),
        m_input(input)
    {}

    std::shared_ptr<InputComponent> GameObject::getInputComponent()
    {
        return m_input;
    }

    std::shared_ptr<VisualComponent> GameObject::getVisualComponent()
    {
        return m_visual;
    }

    void GameObject::end()
    {
        if (m_visual != nullptr) {
            m_visual->getGraphNodePtr()->clear();
        }
        m_visual->move(m_visual->getPosition() * -1);
    }

    GameObject::GameObject(std::shared_ptr<QDUEngine::VisualComponent> &visual, std::shared_ptr<QDUEngine::InputComponent> &input) :
        m_attribute(nullptr),
        m_visual(visual),
        m_input(input)
    {}

    std::pair<std::basic_string<char>, std::basic_string<char>> GameObject::getData()
    {
        return m_visual->getJSON();
    }
}

