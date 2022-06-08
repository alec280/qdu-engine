#include "input_component.hpp"

namespace QDUEngine
{

    InputComponent::InputComponent() = default;

    bool InputComponent::compare(const char *lhs, const char *rhs)
    {
        return std::strcmp(lhs, rhs) == 0;
    }

    void InputComponent::setGameObject(std::shared_ptr<GameObject>& gameObject)
    {
        m_gameObject = gameObject;
    }
}