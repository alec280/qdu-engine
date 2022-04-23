#pragma once
#include "../game_object.hpp"

namespace QDUEngine
{
    class Character : public GameObject {
    public:
        Character(AttributeComponent* attribute, VisualComponent* visual);
    };
}