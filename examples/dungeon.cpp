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

class Static : public QDUEngine::GameObject {
public:
    explicit Static(std::shared_ptr<QDUEngine::VisualComponent>& visual) :
            QDUEngine::GameObject(nullptr, visual)
    {}
};

class Character : public QDUEngine::GameObject {
public:
    explicit Character(std::shared_ptr<QDUEngine::VisualComponent>& visual, std::shared_ptr<QDUEngine::InputComponent>& input) :
        QDUEngine::GameObject(nullptr, visual, input)
    {}
};

class Floor : public QDUEngine::Scene {
public:
    void userStart() noexcept override
    {
        auto blueCube = this->getCube(0, 0, 1);
        auto playerInput = std::make_shared<PlayerInput>(blueCube);
        blueCube->move(QDUEngine::Vector(-2, -2));
        auto player = Character(blueCube, (std::shared_ptr<QDUEngine::InputComponent>&)playerInput);
        this->addGameObject(player);

        auto redCube = this->getCube(1, 0, 0);
        auto enemyInput = std::make_shared<EnemyInput>(redCube);
        redCube->move(QDUEngine::Vector(2, 2));
        auto enemy = Character(redCube, (std::shared_ptr<QDUEngine::InputComponent>&)enemyInput);
        this->addGameObject(enemy);
    }
    void addCompanion()
    {
        auto greenCube = this->getCube(0, 1, 0);
        greenCube->move(this->getGameObjects()[0]->getVisualComponent()->getPosition());
        auto companion = Static(greenCube);
        this->addVisualComponent(companion);
    }
};

class FloorInput : public QDUEngine::InputComponent {
public:
    explicit FloorInput(std::shared_ptr<Floor>& floor) : m_floor(floor) {m_floor->setInputComponent(this);}
    void onAction(const char* action, float value) override {}
    void onCursorAction(const char* action, QDUEngine::Vector2D& pos) override
    {
        if (compare(action, "middleClick")) {
            m_floor->addCompanion();
        }
    }
    std::shared_ptr<Floor> m_floor;
};


int main()
{
    Floor floor1;
    auto sharedFloor = std::make_shared<Floor>(floor1);
    FloorInput floorInput(sharedFloor);
    QDUEngine::Application dungeon;
    dungeon.bindKey("A", "left");
    dungeon.bindKey("W", "up");
    dungeon.bindKey("S", "down");
    dungeon.bindKey("D", "right");
    dungeon.bindJoystick("LS_X", "right");
    dungeon.bindJoystick("LS_Y", "down");
    dungeon.bindCursorButton("LEFT", "customNema");
    dungeon.bindCursorButton("MIDDLE", "middleClick");
    dungeon.run("Dungeon game", QDUEngine::Vector(600, 600), (std::shared_ptr<QDUEngine::Scene>&)sharedFloor);
}
