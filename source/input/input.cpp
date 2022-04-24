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
    }

    void Input::start(
            std::map<const char*, const char*>& keyBindings,
            std::map<const char*, const char*>& joystickBindings
            )
    {
        m_keyBindings = keyBindings;
        m_joystickBindings = joystickBindings;
        for (auto& binding : keyBindings) {
            m_actions.insert(std::pair<const char*, bool>(binding.second, 0));
        }
        for (auto& binding : joystickBindings) {
            m_actions.insert(std::pair<const char*, bool>(binding.second, 0));
        }
        m_window = glfwGetCurrentContext();
    }

    void Input::pollJoysticks(std::map<std::size_t, Joystick>& joysticks)
    {
        for (int joystickId = GLFW_JOYSTICK_1; joystickId < GLFW_JOYSTICK_LAST; ++joystickId)
        {
            int const joystickConnected = glfwJoystickPresent(joystickId);
            if (joystickConnected == GLFW_FALSE)
            {
                if (joysticks.count(joystickId) != 0)
                {
                    joysticks.erase(joystickId);
                }
                continue;
            }

            if (joysticks.count(joystickId) == 0)
            {
                int buttonsCount;
                glfwGetJoystickButtons(joystickId, &buttonsCount);

                int axesCount;
                glfwGetJoystickAxes(joystickId, &axesCount);

                joysticks.emplace(joystickId, Joystick(buttonsCount, axesCount));
            }

            Joystick& joystick = joysticks.at(joystickId);

            int buttonsCount;
            const unsigned char* buttons = glfwGetJoystickButtons(joystickId, &buttonsCount);

            for (int buttonId = 0; buttonId < buttonsCount; ++buttonId)
            {
                joystick.buttons[buttonId] = buttons[buttonId] == GLFW_PRESS;
            }

            int axesCount;
            const float* axes = glfwGetJoystickAxes(joystickId, &axesCount);

            for (int axesId = 0; axesId < axesCount; ++axesId)
            {
                joystick.axes[axesId] = axes[axesId];
            }
        }

        for (auto& elem : joysticks) {
            auto &joystickId = elem.first;
            auto &joystick = elem.second;

            for (auto& binding : m_joystickBindings) {
                if (std::strcmp("LS_X", binding.first) == 0) {
                    float value = joystick.axes[0];
                    if (std::abs(value) > 0.3) {
                        m_actions.at(binding.second) = value * 0.2f;
                    }
                } else if (std::strcmp("LS_Y", binding.first) == 0) {
                    float value = joystick.axes[1];
                    if (std::abs(value) > 0.3) {
                        m_actions.at(binding.second) = value * 0.2f;
                    }
                }
            }
        }
    }

    void Input::update()
    {
        for (auto& action : m_actions) {
            m_actions.at(action.first) = 0;
        }
        glfwPollEvents();
        pollJoysticks(m_joysticks);
        for (auto& action : m_actions) {
            float value = m_actions.at(action.first);
            for (auto& component : m_inputComponents) {
                component->onAction(action.first, value);
            }
        }
    }

    bool Input::checkKey(const char* key, std::pair<const char* const, const char*> binding, int code)
    {
        if (std::strcmp(key, binding.first) == 0 && glfwGetKey(m_window, code) == GLFW_PRESS) {
            std::cout << binding.second << " action activated." << std::endl;
            m_actions.at(binding.second) = 1;
            return true;
        }
        return false;
    }
}

