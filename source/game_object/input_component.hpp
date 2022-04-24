#pragma once

namespace QDUEngine
{
    class GameObject;
    class InputComponent {
    public:
        virtual void onKeyAction(char* action) = 0;
    };
}