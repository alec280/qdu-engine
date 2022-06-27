#include <QDUEngine.hpp>

using namespace QDUEngine;

class PlayerInput : public InputComponent {
public:
    void onAction(Scene* scene, const char* action, float value) override
    {
        auto audio = m_gameObject->getAudioComponent();
        auto visual = m_gameObject->getVisualComponent();
        if (visual == nullptr || audio == nullptr) {
            return;
        }
        if (compare(action, "left")) {
            visual->move(Vector(-value, 0));
            audio->move(Vector(-value, 0));
        } else if (compare(action, "right")) {
            visual->move(Vector(value, 0));
            audio->move(Vector(value, 0));
        } else if (compare(action, "down")) {
            visual->move(Vector(0, value));
            audio->move(Vector(0, value));
        } else if (compare(action, "up")) {
            visual->move(Vector(0, -value));
            audio->move(Vector(0, -value));
        }
    }
    void onCursorAction(const char* action, Vector2D& pos) override {}
    void onUpdate(float timeStep) override {}
};

class EnemyInput : public InputComponent {
public:
    void onAction(Scene* scene, const char* action, float value) override {}
    void onCursorAction(const char* action, Vector2D& pos) override
    {
        auto audio = m_gameObject->getAudioComponent();
        auto visual = m_gameObject->getVisualComponent();
        if (visual == nullptr) {
            return;
        }
        if (compare(action, "leftClick")) {
            if (pos.x < 300) {
                if (audio) {
                    audio->move(Vector(-1, 0));
                }
                visual->move(Vector(-1, 0));
            } else {
                if (audio) {
                    audio->move(Vector(1, 0));
                }
                visual->move(Vector(1, 0));
            }
        }
    }
    void onUpdate(float timeStep) override {}
};

class HunterInput : public InputComponent {
public:
    void onAction(Scene* scene, const char* action, float value) override
    {
        if (compare(action, "hunt")) {
            auto main = scene->getMainObject();
            auto navMesh = scene->getNavigationMesh();
            if (!main || !navMesh){
                return;
            }
            auto from = m_gameObject->getVisualComponent()->getPosition();
            auto to = main->getVisualComponent()->getPosition();
            auto path = navMesh->getPath(from, to);
            m_path.clear();
            for (auto& cell : path) {
                m_path.push_back(navMesh->getCellPosition(cell));
            }
        }
    }
    void onCursorAction(const char* action, Vector2D& pos) override {}
    void onUpdate(float timeStep) override {
        m_delay -= timeStep;
        if (m_delay > 0) {
            return;
        }
        m_delay = 0.1;
        if (!m_path.empty()) {
            auto current = m_gameObject->getVisualComponent()->getPosition();
            auto to = m_path.front();
            auto moveBy = to - current;
            m_gameObject->getVisualComponent()->move(moveBy);
            m_path.erase(std::remove(m_path.begin(), m_path.end(), to), m_path.end());
        }
    }
private:
    float m_delay = 0.1;
    std::vector<Vector2D> m_path{};
};

class SpeedsterInput : public InputComponent {
public:
    void onAction(Scene* scene, const char* action, float value) override {}
    void onCursorAction(const char* action, Vector2D& pos) override {}
    void onUpdate(float timeStep) override
    {
        auto audio = m_gameObject->getAudioComponent();
        auto visual = m_gameObject->getVisualComponent();
        if (visual == nullptr || audio == nullptr) {
            return;
        }
        m_time += timeStep;
        visual->move(Vector(sin(m_time) * 3,0) - visual->getPosition());
        audio->move(Vector3(sin(m_time) * 3,0, 0) - audio->getPosition());
    }
private:
    float m_time = 0.f;
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
    void addCompanion(Vector2D& pos)
    {
        auto enemyInput = std::make_shared<EnemyInput>();
        auto companion = getGameObjectFrom("examples/data/companion.json");
        companion.getVisualComponent()->move(pos);
        auto companionPtr = std::make_shared<GameObject>(companion);
        enemyInput->setGameObject(companionPtr);
        companion.setInputComponent((std::shared_ptr<InputComponent>&)enemyInput);
        auto audio = getAudio("examples/assets/double_bell.wav");
        audio->move(pos);
        companion.setAudioComponent(audio);
        m_scene.addGameObject(companion);
        audio->play();
        std::cout << "Companion added!" << std::endl;
    }
    void addSpeedster()
    {
        auto redCube = getTexturedCube("examples/assets/enemy.png");
        auto speedsterInput = std::make_shared<SpeedsterInput>();
        auto speedster = Character(redCube, (std::shared_ptr<InputComponent>&)speedsterInput);
        auto audio = getAudio("examples/assets/trumpet_mono.wav");
        audio->setAsLooping(true);
        audio->setAs3D(true);
        audio->setRadius(3);
        audio->setAutoPlay(true);
        audio->setPitch(2);
        audio->play();
        speedster.setAudioComponent(audio);
        m_scene.addGameObject(speedster);
        std::cout << "Speedster added!" << std::endl;
    }
    void userStart() noexcept override
    {
        bindCursorButton(Input::CursorButton::LEFT, "leftClick");
        bindCursorButton(Input::CursorButton::MIDDLE, "middleClick");
        bindCursorButton(Input::CursorButton::RIGHT, "rightClick");
        bindKey("A", "left");
        bindKey("W", "up");
        bindKey("S", "down");
        bindKey("D", "right");
        bindKey("M", "map");
        bindKey("P", "pause");
        bindKey("C", "cell");
        bindKey("H", "hunt");
        bindKey("X", "speedster");
        bindJoystick("LS_X", "right");
        bindJoystick("LS_Y", "down");
        bindJoystick("RS_X", "right");
        bindJoystick("RS_Y", "down");
        setTempDir("examples/tmp");

        loadSceneFrom("examples/data/garden.json");
        auto blueCube = getTexturedCube("examples/assets/player.png");
        auto playerInput = std::make_shared<PlayerInput>();
        auto audio = getAudio("");
        audio->setAsListener(true);
        audio->move(Vector(-2, -2));
        blueCube->move(Vector(-2, -2));
        auto player = Character(blueCube, (std::shared_ptr<InputComponent>&)playerInput);
        player.setAudioComponent(audio);
        m_scene.addMainObject(player);

        auto redCube = getTexturedCube("examples/assets/enemy.png");
        auto enemyInput = std::make_shared<EnemyInput>();
        redCube->move(QDUEngine::Vector(2, 2));
        auto enemy = Character(redCube, (std::shared_ptr<InputComponent>&)enemyInput);
        m_scene.addGameObject(enemy);

        auto saveTo = getAbsolutePath("/examples/out/enemy.json");
        saveGameObject(&enemy, saveTo.c_str());
        playAudio("examples/assets/trumpet_mono.wav", false, Vector3(0, 0, 0));
    }
    void onTransition() noexcept override
    {
        if (m_scene.getName() == "obstacles.json" && !m_obstaclesLoaded) {
            setNavigationMesh(&m_scene,
                    "examples/assets/obstacles_nav_mesh.obj",
                    "examples/assets/white.png");
            m_obstaclesLoaded = true;
            auto navMeshVisual = m_scene.getNavigationMesh()->getVisualComponent();
            navMeshVisual->move(Vector(-0.5, -0.5));

            auto redCube = getTexturedCube("examples/assets/enemy.png");
            auto enemyInput = std::make_shared<HunterInput>();
            redCube->move(QDUEngine::Vector(0, 3));
            auto enemy = Character(redCube, (std::shared_ptr<InputComponent>&)enemyInput);
            m_scene.addGameObject(enemy);
        }
    }
private:
    bool m_obstaclesLoaded = false;
};

class GlobalInput : public InputComponent {
public:
    explicit GlobalInput(Dungeon* dungeon) : m_application(dungeon), m_spawnPos(Vector(0, 0)) {}
    void onAction(Scene* scene, const char* action, float) override
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
        } else if (compare(action, "speedster")) {
            m_application->addSpeedster();
        } else if (compare(action, "pause")) {
            m_application->setPaused(!m_application->isPaused());
        } else if (compare(action, "cell")) {
            auto navMesh = m_application->getNavigationMesh();
            auto mainObj = m_application->getMainObject();
            if (navMesh && mainObj) {
                auto pos = mainObj->getVisualComponent()->getPosition();
                std::cout << "Main object is in cell: " << navMesh->getCell(pos) << std::endl;
            }
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
                m_spawnPos += Vector(1, 1);
            }
            m_combo[0] = false;
            m_combo[1] = false;
        }
    }
    void onUpdate(float timeStep) override {}
private:
    bool m_combo[2]{false, false};
    Dungeon* m_application;
    Vector2D m_spawnPos;
};

int main()
{
    auto dungeon = Dungeon();
    auto input = std::make_shared<GlobalInput>(GlobalInput(&dungeon));
    dungeon.setGlobalInput((std::shared_ptr<InputComponent>&)input);
    dungeon.run("Dungeon game", 600, 600);
}
