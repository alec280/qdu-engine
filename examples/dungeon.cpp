#include <QDUEngine.hpp>

class PlayerInput : public QDUEngine::InputComponent {
public:
    explicit PlayerInput(std::shared_ptr<QDUEngine::VisualComponent>& visual) : m_visual(visual) {}
    void onAction(const char* action, float value) override
    {
        if (compare(action, "left")) {
            m_visual->move(QDUEngine::Vector(-value, 0));
        } else if (compare(action, "right")) {
            m_visual->move(QDUEngine::Vector(value, 0));
        } else if (compare(action, "down")) {
            m_visual->move(QDUEngine::Vector(0, value));
        } else if (compare(action, "up")) {
            m_visual->move(QDUEngine::Vector(0, -value));
        }
    }
    void onCursorAction(const char* action, QDUEngine::Vector2D& pos) override {}
    std::shared_ptr<QDUEngine::VisualComponent> m_visual;
};

class Player : public QDUEngine::GameObject {
public:
    explicit Player(std::shared_ptr<QDUEngine::VisualComponent>& visual) :
        QDUEngine::GameObject(nullptr, visual, new PlayerInput(visual))
    {}
};

class EnemyInput : public QDUEngine::InputComponent {
public:
    explicit EnemyInput(std::shared_ptr<QDUEngine::VisualComponent>& visual) : m_visual(visual) {}
    void onAction(const char* action, float value) override {}
    void onCursorAction(const char* action, QDUEngine::Vector2D& pos) override
    {
        if (compare(action, "customNema")) {
            if (pos.x < 300) {
                m_visual->move(QDUEngine::Vector(-1, 0));
            } else {
                m_visual->move(QDUEngine::Vector(1, 0));
            }
        }
    }
    std::shared_ptr<QDUEngine::VisualComponent> m_visual;
};

class Enemy : public QDUEngine::GameObject {
public:
    explicit Enemy(std::shared_ptr<QDUEngine::VisualComponent>& visual) :
        QDUEngine::GameObject(nullptr, visual, new EnemyInput(visual))
    {}
};

class Floor : public QDUEngine::Scene {
public:
    void userStart() noexcept override
    {
        auto redCube = this->getCube(1, 0, 0);
        redCube->move(QDUEngine::Vector(2, 2));
        auto enemy = Enemy(redCube);
        this->addGameObject(enemy);

        auto blueCube = this->getCube(0, 0, 1);
        blueCube->move(QDUEngine::Vector(-2, -2));
        auto player = Player(blueCube);
        this->addGameObject(player);
    }
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
    dungeon.bindCursorButton("LEFT", "customNema");
    dungeon.bindCursorButton("MIDDLE", "middleClick");
    dungeon.run("Dungeon game", QDUEngine::Vector(600, 600), floor1);
}
