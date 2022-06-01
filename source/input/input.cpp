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
            auto string = binding.first;
            auto act = binding.second;
            if (checkKey("A", string, act, GLFW_KEY_A, key, action)) {

            } else if (checkKey("W", string, act, GLFW_KEY_W, key, action)) {

            } else if (checkKey("S", string, act, GLFW_KEY_S, key, action)) {

            } else if (checkKey("D", string, act, GLFW_KEY_D, key, action)) {

            } else if (checkKey("I", string, act, GLFW_KEY_I, key, action)) {

            } else if (checkKey("K", string, act, GLFW_KEY_K, key, action)) {

            } else if (checkKey("L", string, act, GLFW_KEY_L, key, action)) {

            } else if (checkKey("J", string, act, GLFW_KEY_J, key, action)) {

            } else if (checkKey("M", string, act, GLFW_KEY_M, key, action)) {

            } else if (checkKey("U", string, act, GLFW_KEY_U, key, action)) {

            } else if (checkKey("H", string, act, GLFW_KEY_H, key, action)) {

            } else if (checkKey("O", string, act, GLFW_KEY_O, key, action)) {

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
                auto string = binding.first;
                if (checkJoystick("LS_X", string, binding.second, joystick, 0)) {

                } else if (checkJoystick("LS_Y", string, binding.second, joystick, 1)) {

                } else if (checkJoystick("RS_X", string, binding.second, joystick, 2)) {

                } else if (checkJoystick("RS_Y", string, binding.second, joystick, 5)) {

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
                    component->onCursorAction(action.first.c_str(), m_cursorPos);
                }
                m_globalInput->onCursorAction(action.first.c_str(), m_cursorPos);
            }
        }
        for (auto& action : m_actions) {
            float value = m_actions.at(action.first);
            if (std::abs(value) != 0) {
                std::cout << action.first << " action activated." << std::endl;
                for (auto& component : m_inputComponents) {
                    component->onAction(action.first.c_str(), value);
                }
                m_globalInput->onAction(action.first.c_str(), value);
            }
        }
        while (!m_inputComponentsQueue.empty()) {
            auto component = m_inputComponentsQueue.front();
            m_inputComponents.push_back(component);
            m_inputComponentsQueue.erase(m_inputComponentsQueue.begin());
        }
    }

    bool Input::checkJoystick(const char* key, std::string& string, const char* action, Joystick& joystick, int ax)
    {
        if (std::strcmp(key, string.c_str()) == 0) {
            float value = joystick.axes[ax];
            if (std::abs(value) > 0.2) {
                m_actions.at(action) = value * 0.01f;
            }
            return true;
        }
        return false;
    }

    bool Input::checkKey(const char* key, std::string& string, const char* action, int code, int keyGLFW, int actionGLFW)
    {
        if (std::strcmp(key, string.c_str()) == 0 && actionGLFW == GLFW_PRESS && code == keyGLFW) {
            m_actions.at(action) = 1;
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
            if (binding.first == CursorButton::LEFT && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
                m_cursorActions.at(binding.second) = 1;
            } else if (binding.first == CursorButton::MIDDLE && button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
                m_cursorActions.at(binding.second) = 1;
            } else if (binding.first == CursorButton::RIGHT && button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
                m_cursorActions.at(binding.second) = 1;
            }
        }
    }

    void Input::clear()
    {
        m_inputComponentsQueue.clear();
        m_inputComponents.clear();
    }

    void Input::addInputComponent(const std::shared_ptr<InputComponent>& component)
    {
        if (component == nullptr) {
            return;
        }
        m_inputComponentsQueue.push_back(component);
    }
}

