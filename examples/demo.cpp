#include <QDUEngine.hpp>

using namespace QDUEngine;

class PlayerInput : public InputComponent {
public:
    explicit PlayerInput(std::shared_ptr<VisualComponent>& visual) : m_visual(visual) {}
    void onAction(const char* action, float value) override
    {
        if (compare(action, "left")) {
            m_visual->move(Vector(-value, 0));
        } else if (compare(action, "right")) {
            m_visual->move(Vector(value, 0));
        } else if (compare(action, "down")) {
            m_visual->move(Vector(0, value));
        } else if (compare(action, "up")) {
            m_visual->move(Vector(0, -value));
        }
    }
    void onCursorAction(const char* action, Vector2D& pos) override {}
    std::shared_ptr<VisualComponent> m_visual;
};

class EnemyInput : public InputComponent {
public:
    explicit EnemyInput(std::shared_ptr<VisualComponent>& visual) : m_visual(visual) {}
    void onAction(const char* action, float value) override {}
    void onCursorAction(const char* action, Vector2D& pos) override
    {
        if (compare(action, "leftClick")) {
            if (pos.x < 300) {
                m_visual->move(Vector(-1, 0));
            } else {
                m_visual->move(Vector(1, 0));
            }
        }
    }
    std::shared_ptr<VisualComponent> m_visual;
};

class Static : public GameObject {
public:
    explicit Static(std::shared_ptr<VisualComponent>& visual) : GameObject(nullptr, visual) {}
};

class Character : public GameObject {
public:
    explicit Character(std::shared_ptr<VisualComponent>& visual, std::shared_ptr<InputComponent>& input) :
        GameObject(visual, input)
    {}
};

class Dungeon : public Application {
public:
    explicit Dungeon() : Application() {}
    void addCompanion(Vector2D& pos)
    {
        auto greenCube = m_scene->getTexturedCube("examples/assets/companion.png", "companion");
        auto enemyInput = std::make_shared<EnemyInput>(greenCube);
        greenCube->move(pos);
        auto companion = Character(greenCube, (std::shared_ptr<InputComponent>&)enemyInput);
        m_scene->addGameObject(companion);
        std::cout << "Companion added!" << std::endl;
    }
    void userStart() noexcept override
    {
        m_scene->fromJSON("examples/data/garden.json");
        auto blueCube = m_scene->getTexturedCube("examples/assets/player.png", "player");
        auto playerInput = std::make_shared<PlayerInput>(blueCube);
        blueCube->move(QDUEngine::Vector(-2, -2));
        auto player = Character(blueCube, (std::shared_ptr<InputComponent>&)playerInput);
        m_scene->addMainObject(player);

        auto redCube = m_scene->getTexturedCube("examples/assets/enemy.png", "enemy");
        auto enemyInput = std::make_shared<EnemyInput>(redCube);
        redCube->move(QDUEngine::Vector(2, 2));
        auto enemy = Character(redCube, (std::shared_ptr<InputComponent>&)enemyInput);
        m_scene->addGameObject(enemy);
    }
};

class GlobalInput : public InputComponent {
public:
    explicit GlobalInput(Dungeon* dungeon) : m_application(dungeon), m_spawnPos(Vector(0, 0)) {}
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
            m_application->bindKey(&tmp, "left");

            std::cout << "Rebind up: ";
            tmp = (char)std::cin.get();
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore();
            }
            std::cin.ignore(1);
            m_application->bindKey(&tmp, "up");

            std::cout << "Rebind down: ";
            tmp = (char)std::cin.get();
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore();
            }
            std::cin.ignore(1);
            m_application->bindKey(&tmp, "down");

            std::cout << "Rebind right: ";
            tmp = (char)std::cin.get();
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore();
            }
            std::cin.ignore(1);
            m_application->bindKey(&tmp, "right");
        }
    }
    void onCursorAction(const char* action, Vector2D& pos) override
    {
        if (compare(action, "leftClick")) {
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
                m_application->addCompanion(m_spawnPos);
                m_spawnPos += QDUEngine::Vector(1, 1);
            }
            m_combo[0] = false;
            m_combo[1] = false;
        }
    }
    bool m_combo[2]{false, false};
    Dungeon* m_application;
    Vector2D m_spawnPos;
};

int main()
{
    auto dungeon = Dungeon();
    auto input = std::make_shared<GlobalInput>(GlobalInput(&dungeon));
    dungeon.setGlobalInput((std::shared_ptr<InputComponent>&)input);

    dungeon.bindCursorButton(Input::CursorButton::LEFT, "leftClick");
    dungeon.bindCursorButton(Input::CursorButton::MIDDLE, "middleClick");
    dungeon.bindCursorButton(Input::CursorButton::RIGHT, "rightClick");
    dungeon.bindKey("A", "left");
    dungeon.bindKey("W", "up");
    dungeon.bindKey("S", "down");
    dungeon.bindKey("D", "right");
    dungeon.bindKey("M", "map");
    dungeon.bindJoystick("LS_X", "right");
    dungeon.bindJoystick("LS_Y", "down");
    dungeon.bindJoystick("RS_X", "right");
    dungeon.bindJoystick("RS_Y", "down");
    dungeon.preloadJSON("examples/data/garden.json");
    dungeon.preloadJSON("examples/data/warehouse.json");
    dungeon.setTempDir("examples/tmp");
    dungeon.run("Dungeon game", 600, 600);
}
