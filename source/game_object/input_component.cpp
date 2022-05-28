#include "input_component.hpp"

namespace QDUEngine
{
    bool InputComponent::compare(const char *lhs, const char *rhs)
    {
        return std::strcmp(lhs, rhs) == 0;
    }

    void InputComponent::setMain(bool value)
    {
        m_main = value;
    }

    bool InputComponent::isMain()
    {
        return m_main;
    }
}