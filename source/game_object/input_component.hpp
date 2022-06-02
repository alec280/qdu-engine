#pragma once
#include <cstring>
#include <iostream>
#include "../core/vector_2d.hpp"

namespace QDUEngine
{
    class GameObject;
    class InputComponent {
    public:
        virtual void onAction(const char* action, float value) = 0;
        virtual void onCursorAction(const char* action, Vector2D& pos) = 0;
        static bool compare(const char* lhs, const char* rhs);
    };
}