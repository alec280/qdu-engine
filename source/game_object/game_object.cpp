#include "game_object.hpp"

namespace QDUEngine
{
    static int m_current_id;
    static std::hash<std::string> m_hash;

    GameObject::GameObject(AttributeComponent* attribute, std::shared_ptr<VisualComponent>& visual) :
            m_attribute(attribute),
            m_visual(visual),
            m_input(nullptr),
            m_id(std::to_string(m_hash("GameObject" + std::to_string(m_current_id++))))
    {}

    GameObject::GameObject(AttributeComponent* attribute, std::shared_ptr<VisualComponent>& visual, std::shared_ptr<InputComponent>& input) :
            m_attribute(attribute),
            m_visual(visual),
            m_input(input),
            m_id(std::to_string(m_hash("GameObject" + std::to_string(m_current_id++))))
    {
        auto ptr = std::make_shared<GameObject>(*this);
        m_input->setGameObject(ptr);
    }

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

    nlohmann::json GameObject::getData()
    {
        return {
            {"id", getId()},
            {"visual", m_visual->getData()}
        };
    }

    const std::string GameObject::getId()
    {
        return m_id;
    }

    std::shared_ptr<InputComponent> GameObject::getInputComponent()
    {
        return m_input;
    }

    std::shared_ptr<VisualComponent> GameObject::getVisualComponent()
    {
        return m_visual;
    }

    std::ostream &operator<<(std::ostream& ostream, const GameObject& gameObject)
    {
        ostream << "[GameObject:" << gameObject.m_id << "]";
        return ostream;
    }
}

