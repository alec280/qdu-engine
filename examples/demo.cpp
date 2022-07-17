#include <QDUEngine.hpp>

using namespace QDUEngine;

int gameStatus = 0;

class PlayerInput : public InputComponent {
public:
    void onAction(Scene* scene, const char* action, float value) override
    {
        if (gameStatus == -1) {
            return;
        }
        auto audio = m_gameObject->getAudioComponent();
        auto visual = m_gameObject->getVisualComponent();
        if (visual == nullptr || audio == nullptr) {
            return;
        }
        if (value != -1.f && value != 1.f && value != 0.f) {
            if (!m_canUseJoystick) {
                return;
            }
            m_joystickCoolOff = 0.25;
            m_canUseJoystick = false;
        }
        float standardValue = (float)(0 < value) - (float)(value < 0);
        auto potentialMove = Vector2D{};
        if (compare(action, "left")) {
            potentialMove = Vector(-standardValue, 0);
        } else if (compare(action, "right")) {
            potentialMove = Vector(standardValue, 0);
        } else if (compare(action, "down")) {
            potentialMove = Vector(0, standardValue);
        } else if (compare(action, "up")) {
            potentialMove = Vector(0, -standardValue);
        }
        auto potentialPos = visual->getPosition() + potentialMove;
        if (scene->getNavigationMesh()->getCell(potentialPos) == -1) {
            return;
        }
        visual->move(potentialMove);
        audio->move(potentialMove);
    }
    void onCursorAction(const char* action, Vector2D& pos) override {}
    void onUpdate(float timeStep) override
    {
        m_joystickCoolOff -= timeStep;
        if (m_joystickCoolOff <= 0) {
            m_canUseJoystick = true;
        }
    }
private:
    float m_joystickCoolOff = 0.f;
    bool m_canUseJoystick = true;
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
        double posX = sin(m_time) * 0.003;
        visual->move(Vector((float)posX,0));
        audio->move(Vector3((float)posX,0, 0));
    }
private:
    float m_time = 0.f;
};

class EnemyInput : public InputComponent {
public:
    void onAction(Scene* scene, const char* action, float value) override {}
    void onCursorAction(const char* action, Vector2D& pos) override {}
    void onUpdate(float timeStep) override {}
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
        auto greenCube = getTexturedCube("examples/assets/companion.png");
        greenCube->move(Vector(-4, -10));
        auto speedsterInput = std::make_shared<SpeedsterInput>();
        auto speedster = Character(greenCube, (std::shared_ptr<InputComponent>&)speedsterInput);
        auto audio = getAudio("examples/assets/trumpet_mono.wav");
        audio->setAsLooping(true);
        audio->setAs3D(true);
        audio->setRadius(5);
        audio->setAutoPlay(true);
        audio->setPitch(2);
        audio->move(Vector(-4, -10));
        audio->play();
        speedster.setAudioComponent(audio);
        m_scene.addGameObject(speedster);
    }
    void userStart() noexcept override
    {
        // Application settings.
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
        bindKey("LEFT", "cameraLeft");
        bindKey("UP", "cameraUp");
        bindKey("DOWN", "cameraDown");
        bindKey("RIGHT", "cameraRight");
        bindJoystick("LS_X", "right");
        bindJoystick("LS_Y", "down");
        bindJoystick("RS_X", "right");
        bindJoystick("RS_Y", "down");
        setTempDir("examples/tmp");

        // Load most of the objects from disk.
        loadSceneFrom("examples/data/level_1.json");

        // Add main character to the scene.
        auto blueCube = getTexturedCube("examples/assets/player.png");
        auto playerInput = std::make_shared<PlayerInput>();
        auto audio = getAudio("");
        audio->setAsListener(true);
        audio->move(Vector(4, 2));
        blueCube->move(Vector(4, 2));
        moveCamera(Vector3(4, 0, 0));
        auto player = Character(blueCube, (std::shared_ptr<InputComponent>&)playerInput);
        player.setAudioComponent(audio);
        m_scene.addMainObject(player);

        // Add coins to the scene.
        auto coinScale = Vector3(0.5, 0.1, 0.99);
        for (auto& pos : m_coinPos) {
            auto coinCube = getTexturedCube("examples/assets/coin.png");
            coinCube->scale(coinScale);
            coinCube->move(pos);
            auto coin = Static(coinCube);
            m_scene.addGameObject(coin);
            m_coinVisuals.push_back(coinCube);
        }

        // Add enemies to the scene.
        for (auto& pos : m_enemyPos) {
            auto redCube = getTexturedCube("examples/assets/enemy.png");
            redCube->move(pos);
            auto enemyInput = std::make_shared<EnemyInput>();
            auto enemy = Character(redCube, (std::shared_ptr<InputComponent>&)enemyInput);
            m_scene.addGameObject(enemy);
            m_enemyVisuals.push_back(redCube);
        }

        // Add first nav mesh.
        setNavigationMesh(
                &m_scene,
                "examples/assets/level_1_nav_mesh.obj",
                "examples/assets/white.png"
        );
        auto navMeshVisual = m_scene.getNavigationMesh()->getVisualComponent();
        navMeshVisual->move(Vector(-0.5, -0.5));

        // Just to showcase data saving.
        auto saveTo = getAbsolutePath("examples/out/player.json");
        saveGameObject(&player, saveTo.c_str());
        saveTo = getAbsolutePath("examples/out/my_scene.json");
        saveScene(saveTo.c_str());
    }
    void onTransition() noexcept override
    {
        resetCamera();

        // Set second nav mesh.
        if (m_scene.getName() == "level_2.json") {
            setNavigationMesh(&m_scene,
                    "examples/assets/level_2_nav_mesh.obj",
                    "examples/assets/white.png");
            auto navMeshVisual = m_scene.getNavigationMesh()->getVisualComponent();
            navMeshVisual->move(Vector(-0.5, -0.5));
            auto blueCube = getTexturedCube("examples/assets/player.png");
            auto playerInput = std::make_shared<PlayerInput>();
            auto audio = getAudio("");
            audio->setAsListener(true);
            audio->move(Vector(4, 0));
            blueCube->move(Vector(4, 0));
            moveCamera(Vector3(4, 0, 0));
            auto player = Character(blueCube, (std::shared_ptr<InputComponent>&)playerInput);
            player.setAudioComponent(audio);
            m_scene.addMainObject(player);
            addSpeedster();
        }
    }
    std::vector<std::shared_ptr<VisualComponent>> m_coinVisuals{};
    std::vector<std::shared_ptr<VisualComponent>> m_enemyVisuals{};
private:
    std::vector<Vector2D> m_coinPos = {
            {-2, -2},{-2, -1},{-2, -3},{10, -2},{10, -1},{10, -3},
            {5, -6},{6, -6},{2, -6},{3, -6},{4, -6}, {-3, 3},
            {-3, -7}, {11, -7}, {11, 3}
    };
    std::vector<Vector2D> m_enemyPos = {
            {0, -4},{8, -4}
    };
};

class GlobalInput : public InputComponent {
public:
    explicit GlobalInput(Dungeon* dungeon) : m_application(dungeon) {}
    void onAction(Scene* scene, const char* action, float) override
    {
        if (compare(action, "cameraUp")) {
            m_application->moveCamera(Vector3(0, -1, 0));
        } else if (compare(action, "cameraDown")) {
            m_application->moveCamera(Vector3(0, 1, 0));
        } else if (compare(action, "cameraRight")) {
            m_application->moveCamera(Vector3(1, 0, 0));
        } else if (compare(action, "cameraLeft")) {
            m_application->moveCamera(Vector3(-1, 0, 0));
        } else if (compare(action, "map")) {
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
        } else if (compare(action, "pause")) {
            auto paused = m_application->isPaused();
            if (paused) {
                std::cout << "User unpause" << std::endl;
            } else {
                std::cout << "User pause" << std::endl;
            }
            m_application->setPaused(!paused);
        } else if (compare(action, "cell")) {
            auto navMesh = m_application->getNavigationMesh();
            auto mainObj = m_application->getMainObject();
            if (navMesh && mainObj) {
                auto pos = mainObj->getVisualComponent()->getPosition();
                if (gameStatus == -1) {
                    std::cout << "You were in cell: " << navMesh->getCell(pos) << std::endl;
                } else {
                    std::cout << "You are in cell: " << navMesh->getCell(pos) << std::endl;
                }
            }
        } else {
            if (gameStatus != 0) {
                return;
            }
            auto main = scene->getMainObject();
            auto mainPos = main->getVisualComponent()->getPosition();
            int coinIdx = -1;
            for (int i = 0; i < m_application->m_coinVisuals.size(); i++) {
                auto visual = m_application->m_coinVisuals[i];
                if (visual->getPosition() == mainPos) {
                    visual->scale(Vector3(0, 0, 0));
                    coinIdx = i;
                    break;
                }
            }
            if (coinIdx != -1) {
                m_application->playAudio("examples/assets/coin.wav", false, {});
                m_application->m_coinVisuals.erase(m_application->m_coinVisuals.begin() + coinIdx);
            }
            if (m_application->m_coinVisuals.empty()) {
                std::cout << "You won!" << std::endl;
                gameStatus = 1;
                m_application->loadSceneFrom("examples/data/level_2.json");
                m_application->onTransition();
                return;
            }
            if (mainPos != m_lastPos) {
                m_lastPos = mainPos;
                auto newPosVector = std::vector<Vector2D>{};
                auto navMesh = scene->getNavigationMesh();
                for (auto& visual : m_application->m_enemyVisuals) {
                    auto enemyPos = visual->getPosition();
                    auto path = navMesh->getPath(enemyPos, mainPos);
                    if (path.size() > 1) {
                        auto newPos = navMesh->getCellPosition(path[1]);
                        for (auto& otherVisual : m_application->m_enemyVisuals) {
                            if (visual == otherVisual) {
                                continue;
                            }
                            if (newPos == otherVisual->getPosition()) {
                                newPos = 2 * enemyPos - newPos;
                            }
                        }
                        visual->move(newPos - enemyPos);
                        if (newPos == mainPos) {
                            std::cout << "Game Over" << std::endl;
                            main->getVisualComponent()->scale(Vector3(0, 0, 0));
                            gameStatus = -1;
                            break;
                        }
                    }
                }
            }
        }
    }
    void onCursorAction(const char* action, Vector2D& pos) override
    {
        auto zPlane = Vector3(0, 0, 1);
        auto worldPos = m_application->screenToWorld(pos, zPlane, 0);
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
            auto integerPosition = Vector(ceil(worldPos.x), ceil(worldPos.y));
            m_application->addCompanion(integerPosition);
            m_combo[0] = false;
            m_combo[1] = false;
        }
    }
    void onUpdate(float timeStep) override {}
private:
    bool m_combo[2]{false, false};
    Vector2D m_lastPos = {4, 2};
    Dungeon* m_application;
};

int main()
{
    auto dungeon = Dungeon();
    auto input = std::make_shared<GlobalInput>(GlobalInput(&dungeon));
    dungeon.setGlobalInput((std::shared_ptr<InputComponent>&)input);
    dungeon.run("Dungeon game", 600, 600);
}
