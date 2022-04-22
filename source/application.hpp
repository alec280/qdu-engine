#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "scene/scene.hpp"

namespace QDUEngine
{
    class Application {
    public:
        void run(Scene& scene);
        virtual void start(Scene& scene) noexcept = 0;
        virtual void update(Scene& scene) noexcept = 0;
    private:
        Scene m_scene;
    };
}