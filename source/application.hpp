#pragma once
#include "scene/scene.hpp"

namespace QDUEngine
{
    class Application {
    public:
        explicit Application(Scene* scene);
        void run(const char* name, const Vector2D& windowSize);
        void bindCursorButton(const char* key, const char* action);
        void bindKey(const char* key, const char* action);
        void bindJoystick(const char* key, const char* action);
    private:
        Scene* m_scene{};
    };
}
