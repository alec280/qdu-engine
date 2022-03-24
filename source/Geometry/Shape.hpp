#pragma once

#import "../grafica/basic_shapes.h"

namespace QDU {
    Grafica::Shape createColorCube(float r, float g, float b);
    class Shape {
    public:
        Shape(float *vertices, unsigned int verticesSize, float *mIndices, unsigned int indicesSize);

        float *m_vertices{};
        float *m_indices{};
        unsigned int m_vertices_size;
        unsigned int m_indices_size;
        static Shape createColorCube(float r, float g, float b);
    };
}