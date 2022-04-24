#include <QDUEngine.hpp>


class Player : public QDUEngine::GameObject {
public:
    class PlayerInput : public QDUEngine::InputComponent {
        void onAction(const char* action) override
        {
            if (compare(action, "left")) {
                std::cout << "Move left" << std::endl;
            } else if (compare(action, "right")) {
                std::cout << "Move right" << std::endl;
            } else if (compare(action, "down")) {
                std::cout << "Move down" << std::endl;
            } else if (compare(action, "up")) {
                std::cout << "Move up" << std::endl;
            }
        }
    };
    explicit Player(QDUEngine::VisualComponent* visual) :
        QDUEngine::GameObject(nullptr, visual, new PlayerInput)
    {}
};

class Enemy : public QDUEngine::GameObject {
public:
    class NullInput : public QDUEngine::InputComponent {
        void onAction(const char* action) override {}
    };
    explicit Enemy(QDUEngine::VisualComponent* visual) :
        QDUEngine::GameObject(nullptr, visual, new NullInput)
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
    dungeon.bindKey("W", "up");
    dungeon.bindKey("S", "down");
    dungeon.bindKey("D", "right");
    dungeon.bindJoystick("LS_X", "right");
    dungeon.bindJoystick("LS_Y", "down");
    dungeon.run("Dungeon game", QDUEngine::Vector(600, 600), floor1);
}
