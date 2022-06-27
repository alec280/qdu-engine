#pragma once
#include <imgui.h>
#include "../scene/scene.hpp"
#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_glfw.h"

namespace QDUEngine
{
    class DebugManager {
    private:
        void showSceneInformation(Scene* scene);
    };
}