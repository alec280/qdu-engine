#include "input_component.hpp"

namespace QDUEngine
{
    bool InputComponent::compare(const char *lhs, const char *rhs)
    {
        std::cout << rhs << std::endl;
        return std::strcmp(lhs, rhs) == 0;
    }
}