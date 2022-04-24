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

    class Input {
        friend class Scene;
        friend class Window;
    private:
        void keyPressed();
        void start(
                std::map<const char*, const char*>& keyBindings,
                std::map<const char*, const char*>& joystickBindings
                );
        void update();
        bool checkKey(const char* key, std::pair<const char* const, const char*> binding, int code);
        GLFWwindow* m_window{};
        std::vector<std::shared_ptr<InputComponent>> m_inputComponents;
        std::map<const char*, const char*> m_keyBindings{};
        std::map<const char*, const char*> m_joystickBindings{};
        std::map<const char*, float> m_actions{};
        std::map<std::size_t, Joystick> m_joysticks{};
        void joystickCallback(int jid, int event);
        void pollJoysticks(std::map<std::size_t, Joystick> &joysticks);
    };
}