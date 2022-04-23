#include <QDUEngine.hpp>

class Enemy : public QDUEngine::Character {
public:
    explicit Enemy(QDUEngine::VisualComponent* visual) :
        QDUEngine::Character(new QDUEngine::AttributeComponent((char*)"Enemy"), visual)
    {}
    /*
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
    char title[] = "Dungeon game";
    auto window_size = QDUEngine::Vector(600, 600);
    QDUEngine::Scene floor1;
    auto visual = floor1.getCube();
    auto enemy = Enemy(&visual);
    floor1.addGameObject(enemy);
    QDUEngine::Application dungeon;
    dungeon.run(title, window_size, floor1);
}
