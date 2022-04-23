#include <QDUEngine.hpp>

class Enemy : public QDUEngine::Character {
    /*
public:
    Enemy(const std::string& name, float health, float attack) : {};
    virtual void start(QDUEngine::Scene& scene) noexcept
    {

    }
    */
};

class Floor : public QDUEngine::Scene {
    /*
public:
    virtual void start(QDUEngine::Scene& scene) noexcept override
    {
        scene.addGameObject(new Enemy("evil_cube", 10, 1));
    }
    virtual void update(QDUEngine::Scene& scene, float timeStep) noexcept override {}
    */
};

int main()
{
    char name[] = "Dungeon game";
    QDUEngine::Scene floor1;
    system("pause");
    QDUEngine::Vector2D window_size = QDUEngine::Vector(600, 600);
    QDUEngine::Test dungeon(name, window_size);
    //QDUEngine::Application dungeon(name, window_size));
    dungeon.startWindow(name, window_size);
}
