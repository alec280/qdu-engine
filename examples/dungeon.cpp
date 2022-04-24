#include <QDUEngine.hpp>

class PlayerInput : public QDUEngine::InputComponent {
    void onKeyAction(const char* action) override
    {
        std::cout << "Received" << std::endl;
        if (compare(action, "left")) {
            std::cout << "Hewwo!" << std::endl;
        }
    }
};

class Player : public QDUEngine::GameObject {
public:
    explicit Player(QDUEngine::VisualComponent* visual) :
        QDUEngine::GameObject(nullptr, visual, new PlayerInput)
    {}
};

class Enemy : public QDUEngine::GameObject {
public:
    explicit Enemy(QDUEngine::VisualComponent* visual) :
        QDUEngine::GameObject(nullptr, visual, nullptr)
    {}
};

class Floor : public QDUEngine::Scene {
public:
    void userStart() noexcept override
    {
        auto redCube = this->getCube(1, 0, 0);
        redCube.move(QDUEngine::Vector(2, 2));
        auto enemy = Enemy(&redCube);
        this->addGameObject(enemy);

        auto blueCube = this->getCube(0, 0, 1);
        auto player = Player(&blueCube);
        this->addGameObject(player);
    }
    //virtual void update(QDUEngine::Scene& scene, float timeStep) noexcept override {}
};

int main()
{
    Floor floor1;
    QDUEngine::Application dungeon;
    dungeon.bindKey("A", "left");
    dungeon.run("Dungeon game", QDUEngine::Vector(600, 600), floor1);
}
