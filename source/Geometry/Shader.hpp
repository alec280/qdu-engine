#pragma once

#include "Shape.hpp"

#include <glad/glad.h>
#include <iostream>

namespace QDU {
    class GPUShape {
    public:
        GPUShape(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int texture, unsigned int size,
                 Shape shape);
        explicit GPUShape(const Shape& shape);

        unsigned int m_vao{}, m_vbo{}, m_ebo{}, m_texture{}, m_size;
        Shape m_shape;
    };

    class SimpleModelViewProjectionShaderProgram {
    public:
        SimpleModelViewProjectionShaderProgram();
        void drawShape(GPUShape shape) ;
        unsigned int m_shaderProgram;
    };
}