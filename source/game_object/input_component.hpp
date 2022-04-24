#pragma once
#include <cstring>
#include <iostream>

namespace QDUEngine
{
    class GameObject;
    class InputComponent {
    public:
        virtual void onKeyAction(const char* action) = 0;
        static bool compare(const char* lhs, const char* rhs);
    };
}