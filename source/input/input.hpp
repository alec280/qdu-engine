#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "../scene/scene.hpp"

namespace QDUEngine
{
    class Input {
        friend class Application;
        friend class Window;
    public:
        enum CursorButton { LEFT, MIDDLE, RIGHT };
    private:
        struct Joystick
        {
            std::vector<bool> buttons;
            std::vector<float> axes;
            Joystick(std::size_t const numberOfButtons, std::size_t const numberOfAxes):
                    buttons(numberOfButtons, false), axes(numberOfAxes, 0.0F)
            {}
        };
        bool keyPressed(int key, int action);
        void cursorMoved(double xPos, double yPos);
        void update(Scene* scene, float timeStep);
        bool checkKey(const char* key, std::string& string, const char* action, int code, int keyGLFW, int actionGLFW);
        Vector2D m_cursorPos{};
        std::shared_ptr<InputComponent> m_globalInput = nullptr;
        std::map<std::string, std::shared_ptr<InputComponent>> m_loadedComponents{};
        std::map<CursorButton, const char*> m_cursorBindings{};
        std::map<std::string, const char*> m_keyBindings{};
        std::map<std::string, const char*> m_joystickBindings{};
        std::map<std::string, float> m_actions{};
        std::map<std::string, float> m_cursorActions{};
        std::map<std::size_t, Joystick> m_joysticks{};
        void joystickCallback(int jid, int event);
        void pollJoysticks(std::map<std::size_t, Joystick>& joysticks);
        void cursorPressed(int button, int action);
        bool checkJoystick(const char* key, std::string& string, const char* action, Joystick& joystick, int ax);
    };
}