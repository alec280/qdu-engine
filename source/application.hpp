#pragma once
#include "scene/scene.hpp"
#include "scene/window.hpp"

namespace QDUEngine
{
    class Application {
    public:
        void run(char *name, const Vector2D& windowSize, Scene& scene);
    private:
        Scene* m_scene{};
    };
}
