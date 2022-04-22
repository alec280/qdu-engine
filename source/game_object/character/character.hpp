#pragma once
#include "../game_object.hpp"

namespace QDUEngine
{
    class Character : GameObject {
    public:
        Character(AttributeComponent *attribute, VisualComponent* visual);
    };
}