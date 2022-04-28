#pragma once
#include <vector>
#include <map>
#include <GLFW/glfw3.h>
#include <memory>
#include "../game_object/input_component.hpp"

namespace QDUEngine
{
    struct Joystick
    {
        std::vector<bool> buttons;
        std::vector<float> axes;

        Joystick(std::size_t const numberOfButtons, std::size_t const numberOfAxes):
                buttons(numberOfButtons, false), axes(numberOfAxes, 0.0f)
        {}
    };

    struct cmp_str
    {
        bool operator()(char const *a, char const *b) const
        {
            return std::strcmp(a, b) < 0;
        }
    };

    class Input {
        friend class Scene;
        friend class Window;
    private:
        bool keyPressed(int key, int action);
        void cursorMoved(double xPos, double yPos);
        void start();
        void update();
        bool checkKey(const char* key, std::string& string, const char* action, int code, int keyGLFW, int actionGLFW);
        Vector2D m_cursorPos{};
        std::vector<std::shared_ptr<InputComponent>> m_inputComponents;
        std::map<const char*, const char*> m_cursorBindings{};
        std::map<std::string, const char*> m_keyBindings{};
        std::map<const char*, const char*> m_joystickBindings{};
        std::map<const char*, float> m_actions{};
        std::map<const char*, float> m_cursorActions{};
        std::map<std::size_t, Joystick> m_joysticks{};
        void joystickCallback(int jid, int event);
        void pollJoysticks(std::map<std::size_t, Joystick> &joysticks);
        void cursorPressed(int button, int action);
    };
}