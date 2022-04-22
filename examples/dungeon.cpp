#include <QDUEngine.hpp>

class Enemy : public QDUEngine::Character {
public:
    Enemy(const std::string& name, float health, float attack) : {};
    virtual void start(QDUEngine::Scene& scene) noexcept
    {

    }

};

class Floor : public QDUEngine::Scene {
public:
    virtual void start(QDUEngine::Scene& scene) noexcept override
    {
        scene.addGameObject(new Enemy("evil_cube", 10, 1));
    }
    virtual void update(QDUEngine::Scene& scene, float timeStep) noexcept override {}
};

class Dungeon : public QDUEngine::Application {
public:
    virtual void start(QDUEngine::Scene& scene) noexcept override
    {
        scene.addGameObject(new Enemy("evil_cube", 10, 1));
    }
    virtual void update(QDUEngine::Scene& scene, float timeStep) noexcept override {}
};

int main()
{
    Floor floor1;
    Dungeon dungeon;
    dungeon.run(floor1);
}
