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

            } else if (checkKey("I", binding, GLFW_KEY_I, key, action)) {

            } else if (checkKey("K", binding, GLFW_KEY_K, key, action)) {

            } else if (checkKey("L", binding, GLFW_KEY_L, key, action)) {

            } else if (checkKey("J", binding, GLFW_KEY_J, key, action)) {

            } else if (checkKey("M", binding, GLFW_KEY_M, key, action)) {

            } else if (checkKey("U", binding, GLFW_KEY_U, key, action)) {

            } else if (checkKey("H", binding, GLFW_KEY_H, key, action)) {

            } else if (checkKey("O", binding, GLFW_KEY_O, key, action)) {

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

    void Input::start()
    {

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
        for (auto& action : m_cursorActions) {
            m_cursorActions.at(action.first) = 0;
        }
        for (auto& action : m_actions) {
            m_actions.at(action.first) = 0;
        }
        glfwPollEvents();
        pollJoysticks(m_joysticks);
        for (auto& action : m_cursorActions) {
            float value = m_cursorActions.at(action.first);
            if (std::abs(value) != 0) {
                std::cout << action.first << " action activated." << std::endl;
                for (auto& component : m_inputComponents) {
                    component->onCursorAction(action.first, m_cursorPos);
                }
            }
        }
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

    bool Input::checkKey(const char* key, std::pair<const char*, const char*> binding, int code, int keyGLFW, int action)
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
        for (auto& binding : m_cursorBindings) {
            if (std::strcmp("LEFT", binding.first) == 0 && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
                m_cursorActions.at(binding.second) = 1;
            } else if (std::strcmp("MIDDLE", binding.first) == 0 && button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
                m_cursorActions.at(binding.second) = 1;
            } else if (std::strcmp("RIGHT", binding.first) == 0 && button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
                m_cursorActions.at(binding.second) = 1;
            }
        }
    }
}

