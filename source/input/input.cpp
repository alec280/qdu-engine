#include <cstring>
#include <iostream>
#include "input.hpp"

namespace QDUEngine
{
    bool Input::keyPressed(int key, int action)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            return true;
        }
        for (auto& binding : m_keyBindings) {
            if (checkKey("A", binding, GLFW_KEY_A, key, action)) {

            } else if (checkKey("W", binding, GLFW_KEY_W, key, action)) {

            } else if (checkKey("S", binding, GLFW_KEY_S, key, action)) {

            } else if (checkKey("D", binding, GLFW_KEY_D, key, action)) {

            }
        }
        return false;
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
                    if (std::abs(value) > 0.2) {
                        m_actions.at(binding.second) = value * 0.01f;
                    }
                } else if (std::strcmp("LS_Y", binding.first) == 0) {
                    float value = joystick.axes[1];
                    if (std::abs(value) > 0.2) {
                        m_actions.at(binding.second) = value * 0.01f;
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
            if (std::abs(value) != 0) {
                std::cout << action.first << " action activated." << std::endl;
                for (auto& component : m_inputComponents) {
                    component->onAction(action.first, value);
                }
            }
        }
    }

    bool Input::checkKey(const char* key, std::pair<const char* const, const char*> binding, int code, int keyGLFW, int action)
    {
        if (std::strcmp(key, binding.first) == 0 && action == GLFW_PRESS && code == keyGLFW) {
            m_actions.at(binding.second) = 1;
            return true;
        }
        return false;
    }

    void Input::cursorMoved(double xPos, double yPos)
    {
        m_cursorPos = Vector((float)xPos, (float)yPos);
    }

    void Input::cursorPressed(int button, int action)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            std::cout << "Cursor pressed at " << m_cursorPos << std::endl;
        }
    }
}

