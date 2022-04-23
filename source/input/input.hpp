#pragma once
#include <GLFW/glfw3.h>

namespace QDUEngine
{
    class Input {
        friend class Scene;
    public:
        void keyPressed();
    private:
        void start();
        void update();
        GLFWwindow* m_window{};
    };
}