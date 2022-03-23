#pragma once

namespace QDU {
    class World
    {
    public:
        friend class Engine;

        World(const World& world) = delete;

        /**
         * Ends the current application.
         */
        void EndApplication() noexcept;
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
    };
}
#endif