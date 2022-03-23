#pragma once
#ifndef ENGINE_HPP
#define ENGINE_HPP
#include "Application.hpp"
#include "World/World.hpp"

#include <memory>

namespace QDU {
    class Engine
    {
    public:
        explicit Engine(Application& app) : m_world(app) {}
        ~Engine() = default;
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

        /*
		 * Starts the main loop of the engine
		 */
        void StartMainLoop() noexcept
        {
            m_world.StartMainLoop();
        }
    private:
        World m_world;
    };
}

#endif