#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../core/vector_2d.hpp"
#include "../grafica/easy_shaders.h"
#include "../grafica/simple_eigen.h"
#include "../grafica/transformations.h"

namespace QDUEngine
{
    class Window {
    public:
        static void start(char *name, QDUEngine::Vector2D& window_size);
    };
}