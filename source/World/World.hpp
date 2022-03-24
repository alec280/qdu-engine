#pragma once
#ifndef WORLD_HPP
#define WORLD_HPP

#include "../Application.hpp"

#include <GLFW/glfw3.h>

namespace QDU {
    class World
    {
    public:
        friend class Engine;

        World(const World& world) = delete;

        /**
         * Gets the current application window.
         */
        GLFWwindow* GetWindow() noexcept;

        /**
         * Sets the current application window.
         */
        void SetWindow(char* name) noexcept;

        /**
         * Ends the current application.
         */
        void EndApplication() noexcept;

        void ProcessInput() noexcept;

        unsigned int GetShaderProgram() const;

        void SetShaderProgram(unsigned int mShaderProgram);

        unsigned int GetVAO() const;

        void SetVAO(unsigned int vao);
    private:
        explicit World(Application& app);

        /**
         * World destructor.
         */
        ~World();

        /**
         * Starts the main loop for this world.
         */
        void StartMainLoop() noexcept;

        /**
         * Updates the world.
         */
        void Update(float timeStep) noexcept;

        Application &m_application;
        bool m_shouldClose;
        GLFWwindow* m_window;
        unsigned int m_shaderProgram, m_vao;
    };
}
#endif