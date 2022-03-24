#pragma once
#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <memory>
#include <glad/glad.h>

namespace QDU {
    class World;
    class Application
    {
    public:
        friend class World;

        /**
		 * Virtual function that the engine has to implement. This method is called during the start process of the engine.
		 */
        virtual void UserStartUp(World &world) noexcept = 0;

        /**
         * Virtual function that the engine has to implement. This method is called during the end process of the engine.
         */
        virtual void UserShutDown(World& world) noexcept = 0;

        /**
		 * Virtual function that the engine has to implement. This method is called at every iteration of the main loop, the user has to
		 * set all the logic of the game here.
		 */
        virtual void UserUpdate(World& world, float timeStep)  noexcept = 0;

        virtual ~Application() = default;
    private:
        /**
		 * Executes the world start up.
		 */
        void StartUp(World& world) noexcept;
    };
}
#endif