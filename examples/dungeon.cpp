#include <QDUEngine.hpp>

class Enemy : public QDUEngine::Character {
public:
    explicit Enemy(QDUEngine::VisualComponent* visual) :
        QDUEngine::Character(nullptr, visual)
    {}
    /*
    virtual void start(QDUEngine::Scene& scene) noexcept
    {

    }
    */
};

class Floor : public QDUEngine::Scene {
public:
    void userStart() noexcept override
    {
        auto visual = this->getCube(1, 0, 0);
        visual.move(QDUEngine::Vector(2, 2));
        auto enemy = Enemy(&visual);
        this->addGameObject(enemy);
    }
    //virtual void update(QDUEngine::Scene& scene, float timeStep) noexcept override {}
};

int main()
{
    char title[] = "Dungeon game";
    auto window_size = QDUEngine::Vector(600, 600);
    Floor floor1;
    QDUEngine::Application dungeon;
    dungeon.run(title, window_size, floor1);
}
