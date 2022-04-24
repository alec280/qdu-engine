#include <cstring>
#include <iostream>
#include "input.hpp"

namespace QDUEngine
{
    void Input::keyPressed()
    {
        if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(m_window, true);
        }
        for (auto& binding : m_keyBindings) {
            if (checkKey("A", binding, GLFW_KEY_A)) {

            } else if (checkKey("W", binding, GLFW_KEY_W)) {

            } else if (checkKey("S", binding, GLFW_KEY_S)) {

            } else if (checkKey("D", binding, GLFW_KEY_D)) {

            }
        }
    }

    void Input::joystickCallback(int jid, int event)
    {
        if (event == GLFW_CONNECTED)
        {
            std::cout << "The joystick " << jid << " was connected" << std::endl;
        }
        else if (event == GLFW_DISCONNECTED)
        {
            std::cout << "The joystick " << jid << " was disconnected" << std::endl;
        }
        int count = 0;
        const float* values = glfwGetJoystickAxes(jid, &count);
        std::cout << count << std::endl;
        /*
        if (count == 0) {
            return;
        }
        for (int i = 0; i < count; i++) {
            std::cout << values[i] << std::endl;
        }
        */
    }

    void Input::start(
            std::map<const char*, const char*>& keyBindings,
            std::map<const char*, const char*>& joystickBindings
            )
    {
        m_keyBindings = keyBindings;
        m_joystickBindings = joystickBindings;
        for (auto& binding : keyBindings) {
            m_actions.insert(std::pair<const char*, bool>(binding.second, false));
        }
        for (auto& binding : joystickBindings) {
            m_actions.insert(std::pair<const char*, bool>(binding.second, false));
        }
        m_window = glfwGetCurrentContext();
    }

    void Input::update()
    {
        for (auto& action : m_actions) {
            m_actions.at(action.first) = false;
        }
        glfwPollEvents();
        for (auto& action : m_actions) {
            if (m_actions.at(action.first)) {
                for (auto& component : m_inputComponents) {
                    component->onAction(action.first);
                }
            }
        }
    }

    bool Input::checkKey(const char* key, std::pair<const char* const, const char*> binding, int code)
    {
        if (std::strcmp(key, binding.first) == 0 && glfwGetKey(m_window, code) == GLFW_PRESS) {
            std::cout << binding.second << " action activated." << std::endl;
            m_actions.at(binding.second) = true;
            return true;
        }
        return false;
    }
}

