#include <iostream>
#include "input.hpp"

namespace QDUEngine
{
    void Input::keyPressed()
    {
        if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(m_window, true);
        }
    }

    void Input::start()
    {
        m_window = glfwGetCurrentContext();
    }

    void Input::update()
    {
        glfwPollEvents();
    }
}

