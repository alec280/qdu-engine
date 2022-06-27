#pragma once
#include <imgui.h>
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../scene/scene.hpp"

namespace QDUEngine
{
    class DebugManager {
        friend class Application;
    private:
        void end() noexcept;
        void showSceneInformation(Scene* scene);
        void start();
    };
}