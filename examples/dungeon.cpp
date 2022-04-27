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
        greenCube->move(this->getGameObject(0)->getVisualComponent()->getPosition());
        auto companion = Static(greenCube);
        this->addVisualComponent(companion);
    }
};

class FloorInput : public QDUEngine::InputComponent {
public:
    explicit FloorInput(Floor* floor) : m_floor(floor) {m_floor->setInputComponent(this);}
    void onAction(const char* action, float value) override {}
    void onCursorAction(const char* action, QDUEngine::Vector2D& pos) override
    {
        if (compare(action, "customNema")) {
            m_combo[0] = pos.x < 200;
            m_combo[1] = false;
        } else if (compare(action, "middleClick")) {
            if (!m_combo[0] || m_combo[1]) {
                m_combo[1] = false;
            } else if (pos.x > 200 && pos.x < 400) {
                m_combo[1] = true;
            } else {
                m_combo[0] = false;
            }
        } else if (compare(action, "rightClick")) {
            if (!m_combo[0] || !m_combo[1]) {
                return;
            }
            if (pos.x > 400) {
                m_floor->addCompanion();
            }
            m_combo[0] = false;
            m_combo[1] = false;
        }
    }
    bool m_combo[2]{false, false};
    Floor* m_floor;
};


int main()
{
    Floor floor1;
    FloorInput floorInput(&floor1);
    QDUEngine::Application dungeon;
    dungeon.bindKey("A", "left");
    dungeon.bindKey("W", "up");
    dungeon.bindKey("S", "down");
    dungeon.bindKey("D", "right");
    dungeon.bindJoystick("LS_X", "right");
    dungeon.bindJoystick("LS_Y", "down");
    dungeon.bindCursorButton("LEFT", "customNema");
    dungeon.bindCursorButton("MIDDLE", "middleClick");
    dungeon.bindCursorButton("RIGHT", "rightClick");
    dungeon.run("Dungeon game", QDUEngine::Vector(600, 600), &floor1);
}
