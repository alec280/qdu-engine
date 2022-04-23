#pragma once
#include "../game_object/game_object.hpp"

namespace QDUEngine
{
    class Scene {
        friend class Application;
    public:
        void addGameObject(GameObject& gameObject);
    private:
        void start();
        void update();
    };
}