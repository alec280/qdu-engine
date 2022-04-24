#include "game_object.hpp"

namespace QDUEngine
{
    GameObject::GameObject(AttributeComponent* attribute, std::shared_ptr<QDUEngine::VisualComponent>& visual, InputComponent* input) :
        m_attribute(attribute),
        m_visual(visual),
        m_input(input)
    {}

    void GameObject::update(Map& map)
    {
        //m_attribute->update(*this);
        //m_visual->update(*this, map);
    }

    std::shared_ptr<InputComponent> GameObject::getInputComponent()
    {
        return std::shared_ptr<InputComponent>(m_input);
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
    }
}

