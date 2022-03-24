#include "Shader.hpp"

namespace QDU {
    GPUShape::GPUShape(
        unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int texture, unsigned int size, Shape shape) :
        m_vao(vao), m_vbo(vbo), m_ebo(ebo), m_texture(texture), m_size(size), m_shape(shape) {}

    GPUShape::GPUShape(const Shape &shape) :
        m_size(shape.m_indices_size),
        m_shape(shape)
        {
            glGenVertexArrays(1, &m_vao);
            glGenBuffers(1, &m_vbo);
            glGenBuffers(1, &m_ebo);
            glBindVertexArray(m_vao);

            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, shape.m_vertices_size, shape.m_vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.m_indices_size, shape.m_indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

    SimpleModelViewProjectionShaderProgram::SimpleModelViewProjectionShaderProgram() {
        const char *vertexShaderSource = "#version 330 core\n"
                                         "uniform mat4 model;\n"
                                         "uniform mat4 view;\n"
                                         "uniform mat4 projection;\n"

                                         "layout (location = 0) in vec3 position;\n"
                                         "in vec3 color;\n"

                                         "out vec3 newColor;\n"

                                         "void main()\n"
                                         "{\n"
                                         "   gl_Position = projection * view * model * vec4(position, 1.0f);\n"
                                         "   newColor = color;\n"
                                         "}\0";

        const char *fragmentShaderSource = "#version 330 core\n"
                                           "in vec3 newColor;\n"
                                           "out vec4 outColor;\n"
                                           "void main()\n"
                                           "{\n"
                                           "   outColor = vec4(newColor, 1.0f);\n"
                                           "}\n\0";

        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);

        int success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

        if (!success) {
            std::cout << "Error while compiling vertexShader" << std::endl;
        }

        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

        if (!success) {
            std::cout << "Error while compiling fragmentShader" << std::endl;
        }

        m_shaderProgram = glCreateProgram();
        glAttachShader(m_shaderProgram, vertexShader);
        glAttachShader(m_shaderProgram, fragmentShader);
        glLinkProgram(m_shaderProgram);

        glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);

        if (!success) {
            std::cout << "Error while linking shaderProgram" << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void SimpleModelViewProjectionShaderProgram::drawShape(GPUShape shape) {
        glBindVertexArray(shape.m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, shape.m_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape.m_ebo);

        unsigned int position = glGetAttribLocation(m_shaderProgram, "position");
        glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 24, nullptr);
        glEnableVertexAttribArray(position);

        unsigned int color = glGetAttribLocation(m_shaderProgram, "color");
        glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 24, reinterpret_cast<const void *>(12));
        glEnableVertexAttribArray(color);

        glDrawElements(GL_TRIANGLES, (int)shape.m_size, GL_UNSIGNED_INT, nullptr);
    }
}