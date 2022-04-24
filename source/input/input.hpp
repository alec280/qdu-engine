#pragma once
#include <vector>
#include <map>
#include <GLFW/glfw3.h>
#include <memory>
#include "../game_object/input_component.hpp"

namespace QDUEngine
{
    class Input {
        friend class Scene;
        friend class Window;
    private:
        void keyPressed();
        void start(std::map<const char*, const char*>& bindings);
        void update();
        GLFWwindow* m_window{};
        std::vector<std::shared_ptr<InputComponent>> m_inputComponents;
        std::map<const char*, const char*> m_keyBindings{};
    };
}