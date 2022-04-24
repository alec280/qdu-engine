#include "input_component.hpp"

namespace QDUEngine
{
    bool InputComponent::compare(const char *lhs, const char *rhs)
    {
        system("pause");
        std::cout << rhs << std::endl;
        system("pause");
        return std::strcmp(lhs, rhs) == 0;
    }

    void InputComponent::onKeyAction(const char *action)
    {

    }
}