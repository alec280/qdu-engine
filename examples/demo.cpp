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
        QDUEngine::GameObject(visual, input)
    {}
};

class Floor : public QDUEngine::Scene {
public:
    void userStart() noexcept override
    {
        fromJSON("examples/data/garden.json");
        auto blueCube = this->getTexturedCube("examples/assets/player.png", "player");
        auto playerInput = std::make_shared<PlayerInput>(blueCube);
        blueCube->move(QDUEngine::Vector(-2, -2));
        auto player = Character(blueCube, (std::shared_ptr<QDUEngine::InputComponent>&)playerInput);
        this->addMainObject(player);

        auto redCube = this->getTexturedCube("examples/assets/enemy.png", "enemy");
        auto enemyInput = std::make_shared<EnemyInput>(redCube);
        redCube->move(QDUEngine::Vector(2, 2));
        auto enemy = Character(redCube, (std::shared_ptr<QDUEngine::InputComponent>&)enemyInput);
        this->addGameObject(enemy);
    }
    void addCompanion(QDUEngine::Vector2D& pos)
    {
        auto greenCube = this->getTexturedCube("examples/assets/companion.png", "companion");
        auto enemyInput = std::make_shared<EnemyInput>(greenCube);
        greenCube->move(pos);
        auto companion = Character(greenCube, (std::shared_ptr<QDUEngine::InputComponent>&)enemyInput);
        this->addGameObject(companion);
        std::cout << "Companion added!" << std::endl;
    }
};

class FloorInput : public QDUEngine::InputComponent {
public:
    explicit FloorInput(Floor* floor) : m_floor(floor), m_spawn(QDUEngine::Vector(0, 0)) {m_floor->setInputComponent(this);}
    void onAction(const char* action, float value) override
    {
        if (compare(action, "map")) {
            std::cout << "Rebind left: ";
            char tmp = (char)std::cin.get();
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore();
            }
            std::cin.ignore(1);
            m_floor->bindKey(&tmp, "left");

            std::cout << "Rebind up: ";
            tmp = (char)std::cin.get();
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore();
            }
            std::cin.ignore(1);
            m_floor->bindKey(&tmp, "up");

            std::cout << "Rebind down: ";
            tmp = (char)std::cin.get();
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore();
            }
            std::cin.ignore(1);
            m_floor->bindKey(&tmp, "down");

            std::cout << "Rebind right: ";
            tmp = (char)std::cin.get();
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore();
            }
            std::cin.ignore(1);
            m_floor->bindKey(&tmp, "right");
        }
    }
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
                m_floor->addCompanion(m_spawn);
                m_spawn += QDUEngine::Vector(1, 1);
            }
            m_combo[0] = false;
            m_combo[1] = false;
        }
    }
    bool m_combo[2]{false, false};
    Floor* m_floor;
    QDUEngine::Vector2D m_spawn;
};


int main()
{
    Floor floor1;
    FloorInput floorInput(&floor1);
    QDUEngine::Application dungeon(&floor1);
    dungeon.bindKey("A", "left");
    dungeon.bindKey("W", "up");
    dungeon.bindKey("S", "down");
    dungeon.bindKey("D", "right");
    dungeon.bindKey("M", "map");
    dungeon.bindJoystick("LS_X", "right");
    dungeon.bindJoystick("LS_Y", "down");
    dungeon.bindJoystick("RS_X", "right");
    dungeon.bindJoystick("RS_Y", "down");
    dungeon.bindCursorButton("LEFT", "customNema");
    dungeon.bindCursorButton("MIDDLE", "middleClick");
    dungeon.bindCursorButton("RIGHT", "rightClick");
    dungeon.preloadJSON("examples/data/garden.json");
    dungeon.preloadJSON("examples/data/warehouse.json");
    dungeon.setTempDir("examples/tmp");
    dungeon.run("Dungeon game", 600, 600);
}
