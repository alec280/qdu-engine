#include "Shape.hpp"

namespace QDU {
    Shape::Shape(float *vertices, unsigned int verticesSize, float *indices,  unsigned int indicesSize) :
    m_vertices(vertices), m_indices(indices), m_vertices_size(verticesSize), m_indices_size(indicesSize) {}

    Shape Shape::createColorCube(float r, float g, float b) {
        float vertices[48] = {
            -0.5, -0.5,  0.5, r, g, b,
             0.5, -0.5,  0.5, r, g, b,
             0.5,  0.5,  0.5, r, g, b,
            -0.5,  0.5,  0.5, r, g, b,
            -0.5, -0.5, -0.5, r, g, b,
             0.5, -0.5, -0.5, r, g, b,
             0.5,  0.5, -0.5, r, g, b,
            -0.5,  0.5, -0.5, r, g, b
        };
        float indices[36] = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            4, 5, 1, 1, 0, 4,
            6, 7, 3, 3, 2, 6,
            5, 6, 2, 2, 1, 5,
            7, 4, 0, 0, 3, 7
        };
        return {vertices, 48, indices, 36};
    }
}