#include "input.hpp"

namespace QDUEngine
{
    void Input::keyPressed()
    {
        if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(m_window, true);
        }
    }

    void Input::start(std::map<const char*, const char*>& bindings)
    {
        m_keyBindings = bindings;
        m_window = glfwGetCurrentContext();
    }

    void Input::update()
    {
        glfwPollEvents();
    }
}

