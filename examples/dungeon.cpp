#include <QDUEngine.hpp>

class PlayerInput : public QDUEngine::InputComponent {
    void onKeyAction(char* action) override
    {

    }
};


class Enemy : public QDUEngine::GameObject {
public:
    explicit Enemy(QDUEngine::VisualComponent* visual) :
        QDUEngine::GameObject(nullptr, visual, nullptr)
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
    Floor floor1;
    QDUEngine::Application dungeon;
    dungeon.run((char*)"Dungeon game", QDUEngine::Vector(600, 600), floor1);
}
