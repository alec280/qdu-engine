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
        visual->move(Vector(sin(m_time) * 3,-10) - visual->getPosition());
        audio->move(Vector3(sin(m_time) * 3,-10, 0) - audio->getPosition());
    }
private:
    float m_time = 0.f;
};

class BombInput : public InputComponent {
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
    void spawnBomb(Vector2D& pos)
    {
        if (m_bombUsed) {
            return;
        }
        if (m_bomb != nullptr) {
            auto audio = m_bomb->getAudioComponent();
            auto visual = m_bomb->getVisualComponent();
            visual->scale(Vector3(0.5, 0.5, 0.5));
            visual->move(pos - visual->getPosition());
            playAudio("examples/assets/double_bell.wav", false, {});
            return;
        }
        auto bombInput = std::make_shared<BombInput>();
        auto bomb = getGameObjectFrom("examples/data/bomb.json");
        bomb.getVisualComponent()->move(pos);
        auto bombPtr = std::make_shared<GameObject>(bomb);
        bombInput->setGameObject(bombPtr);
        bomb.setInputComponent((std::shared_ptr<InputComponent>&)bombInput);
        playAudio("examples/assets/double_bell.wav", false, {});
        m_scene.addGameObject(bomb);
        m_bomb = bombPtr;
        std::cout << "Bomb dropped!" << std::endl;
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
    void startGame()
    {
        // Clean previous.
        m_coinVisuals.clear();
        m_enemyVisuals.clear();
        m_bomb = nullptr;
        m_bombUsed = false;
        resetCamera();
        gameStatus = 0;

        // Load most objects from disk.
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
            auto enemy = Static(redCube);
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
        bindKey("R", "reset");
        bindKey("ENTER", "bomb");
        bindKey("LEFT", "cameraLeft");
        bindKey("UP", "cameraUp");
        bindKey("DOWN", "cameraDown");
        bindKey("RIGHT", "cameraRight");
        bindJoystick("LS_X", "right");
        bindJoystick("LS_Y", "down");
        bindJoystick("RS_X", "right");
        bindJoystick("RS_Y", "down");
        setTempDir("examples/tmp");

        // Add gameObjects.
        startGame();

        // Just to showcase data saving.
        auto saveTo = getAbsolutePath("examples/out/player.json");
        saveGameObject(m_scene.getMainObject().get(), saveTo.c_str());
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
    bool m_bombUsed = false;
    std::shared_ptr<GameObject> m_bomb = nullptr;
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
            std::string newKey;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore();
            }
            std::cin.ignore(1);
            newKey = std::string(&tmp).at(0);
            m_application->bindKey(newKey, "left");

            std::cout << "Rebind up: ";
            tmp = (char)std::cin.get();
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore();
            }
            std::cin.ignore(1);
            newKey = std::string(&tmp).at(0);
            m_application->bindKey(newKey, "up");

            std::cout << "Rebind down: ";
            tmp = (char)std::cin.get();
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore();
            }
            std::cin.ignore(1);
            newKey = std::string(&tmp).at(0);
            m_application->bindKey(newKey, "down");

            std::cout << "Rebind right: ";
            tmp = (char)std::cin.get();
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore();
            }
            std::cin.ignore(1);
            newKey = std::string(&tmp).at(0);
            m_application->bindKey(newKey, "right");
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
        } else if (compare(action, "bomb") && gameStatus == 0 && m_selectedCell != -1) {
            auto pos = m_application->getNavigationMesh()->getCellPosition(m_selectedCell);
            m_application->spawnBomb(pos);
        } else if (compare(action, "reset") && gameStatus == -1) {
            m_application->startGame();
        } else {
            if (gameStatus != 0) {
                return;
            }
            auto main = scene->getMainObject();
            auto mainPos = main->getVisualComponent()->getPosition();
            if (mainPos == m_lastPos) {
                return;
            }
            m_lastPos = mainPos;
            auto bomb = m_application->m_bomb;
            if (bomb) {
                if (mainPos == bomb->getVisualComponent()->getPosition()) {
                    bomb->getVisualComponent()->scale(Vector3(0, 0, 0));
                }
            }
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
            auto newPosVector = std::vector<Vector2D>{};
            auto navMesh = scene->getNavigationMesh();
            for (auto& visual : m_application->m_enemyVisuals) {
                if (visual->getScale() == Vector3(0, 0, 0)) {
                    continue;
                }
                auto enemyPos = visual->getPosition();
                if (bomb) {
                    auto bombVisual = bomb->getVisualComponent();
                    if (enemyPos == bombVisual->getPosition() &&
                    bombVisual->getScale() != Vector3(0, 0, 0) && !m_application->m_bombUsed) {
                        visual->scale(Vector3(0, 0, 0));
                        bomb->getVisualComponent()->scale(Vector3(0, 0, 0));
                        m_application->m_bombUsed = true;
                        continue;
                    }
                }
                auto path = navMesh->getPath(enemyPos, mainPos);
                if (path.size() > 1) {
                    auto newPos = navMesh->getCellPosition(path[1]);
                    for (auto& otherVisual : m_application->m_enemyVisuals) {
                        if (visual == otherVisual) {
                            continue;
                        }
                        if (newPos == otherVisual->getPosition() && otherVisual->getScale().x != 0) {
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
    void onCursorAction(const char* action, Vector2D& pos) override
    {
        if (gameStatus != 0) {
            return;
        }
        if (compare(action, "leftClick")) {
            auto zPlane = Vector3(0, 0, 1);
            auto worldPos = m_application->screenToWorld(pos, zPlane, 0);
            auto worldPos2D = Vector(worldPos.x, worldPos.y);
            m_selectedCell = m_application->getNavigationMesh()->getCell(worldPos2D);
        }
    }
    void onUpdate(float timeStep) override {}
private:
    Vector2D m_lastPos = {4, 2};
    int m_selectedCell = -1;
    Dungeon* m_application;
};

int main()
{
    auto dungeon = Dungeon();
    auto input = std::make_shared<GlobalInput>(GlobalInput(&dungeon));
    dungeon.setGlobalInput((std::shared_ptr<InputComponent>&)input);
    dungeon.run("Dungeon game", 600, 600);
}
