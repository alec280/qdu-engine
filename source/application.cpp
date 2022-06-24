#include "application.hpp"

namespace QDUEngine
{
    Application::Application() = default;

    void Application::bindCursorButton(Input::CursorButton cursorButton, const char* action)
    {
        m_input.m_cursorBindings[cursorButton] = action;
        m_input.m_cursorActions[std::string(action)] = 0;
    }

    void Application::bindKey(const char* key, const char* action)
    {
        m_input.m_keyBindings[std::string(key)] = action;
        m_input.m_actions[std::string(action)] = 0;
    }

    void Application::bindJoystick(const char* key, const char* action)
    {
        m_input.m_joystickBindings[std::string(key)] = action;
        m_input.m_actions[std::string(action)] = 0;
    }

    void Application::doTransition()
    {
        auto main = m_scene.m_mainObject;
        if (main == nullptr) {
            return;
        }
        std::string toScene;
        Vector2D toPos{};
        auto pos = main->getVisualComponent()->getPosition();
        for (auto& transition : m_scene.m_transitions) {
            if (pos == transition.second.first) {
                toScene = transition.first;
                toPos = transition.second.second;
                break;
            }
        }
        if (!toScene.empty()) {
            loadSceneFrom(toScene.c_str());
            auto audio = main->getAudioComponent();
            if (audio) {
                audio->move(toPos);
            }
            auto visual = main->getVisualComponent();
            if (visual) {
                visual->move(toPos);
            }
            m_scene.addMainObject(main);
            onTransition();
        }
    }

    std::string Application::getAbsolutePath(const char *path)
    {
        return Grafica::getPath(path).string();
    }

    std::shared_ptr<AudioComponent> Application::getAudio(const char* audioPath)
    {
        return m_audio.getAudio(audioPath);
    }

    std::shared_ptr<VisualComponent> Application::getTexturedMesh(const char* objPath, const char* texturePath)
    {
        return m_window.getMesh(objPath, texturePath);
    }

    GameObject Application::getGameObjectFrom(const char* path)
    {
        log("Loading new game object from file.");
        auto fullPath = Grafica::getPath(path);
        auto data = nlohmann::json::parse(std::ifstream(fullPath));
        auto visual = data["visual"];
        auto cube = loadVisualComponent(visual);
        log("New game object loaded from file.");
        return {nullptr, cube};
    }

    GameObject Application::getGameObjectFrom(const char* path, std::shared_ptr<InputComponent>& input)
    {
        log("Loading new game object from file.");
        auto fullPath = Grafica::getPath(path);
        auto data = nlohmann::json::parse(std::ifstream(fullPath));
        auto visual = data["visual"];
        auto cube = loadVisualComponent(visual);
        log("New game object loaded from file.");
        return {nullptr, cube, input};
    }

    Scene Application::getSceneFrom(const char* path)
    {
        if (m_tempDir == nullptr) {
            log("Temp directory not set. Will get an empty scene.");
            return {};
        }
        log("Loading scene from file.");
        auto newScene = Scene();
        auto fullPath = Grafica::getPath(path);
        auto sceneName = fullPath.filename().string();
        auto fileName = "/" + sceneName;
        auto tempPath = Grafica::getPath(m_tempDir + fileName);
        newScene.m_name = sceneName;
        newScene.m_source = path;
        nlohmann::json data;
        if (std::filesystem::exists(tempPath)) {
            data = nlohmann::json::parse(std::ifstream(tempPath));
        } else {
            data = nlohmann::json::parse(std::ifstream(fullPath));
        }
        auto objects = data["objects"];
        for (const auto& objectData : objects) {
            auto visual = objectData.value("visual", nlohmann::json::object());
            std::shared_ptr<VisualComponent> cube = nullptr;
            if (!visual.empty()) {
                cube = loadVisualComponent(visual);
            }
            auto audio = objectData.value("audio", nlohmann::json::object());
            std::shared_ptr<AudioComponent> audioPtr = nullptr;
            if (!audio.empty()) {
                audioPtr = getAudio(audio["source"].get<std::string>().c_str());
                audioPtr->move(Vector3(audio["posX"].get<float>(), audio["posY"].get<float>(), audio["posZ"].get<float>()));
                audioPtr->setAsListener(audio["isListener"]);
                audioPtr->setAs3D(audio["is3D"]);
                audioPtr->setAsLooping(audio["loop"]);
                audioPtr->setRadius(audio["radius"]);
                audioPtr->setPitch(audio["pitch"]);
                if (audio["autoPlay"].get<bool>()) {
                    audioPtr->setAutoPlay(audio["autoPlay"]);
                    audioPtr->play();
                }
            }
            std::shared_ptr<InputComponent> input = nullptr;
            auto objectId = objectData.value("id", "");
            for (auto& element : m_input.m_loadedComponents) {
                if (element.first == objectId && !element.first.empty()) {
                    input = element.second;
                    break;
                }
            }
            auto object = GameObject(nullptr, cube);
            if (!objectId.empty()) {
                object.m_id = objectId;
            }
            auto objectPtr = std::make_shared<GameObject>(object);
            if (audio != nullptr) {
                objectPtr->setAudioComponent(audioPtr);
            }
            if (input != nullptr) {
                input->setGameObject(objectPtr);
            }
            objectPtr->setInputComponent(input);
            newScene.m_gameObjectsQueue.push_back(objectPtr);
        }
        auto transitions = data["transitions"];
        for (auto it = transitions.begin(); it != transitions.end(); ++it) {
            auto toSceneName = it.key();
            auto transitionData = it.value();
            auto fromVector = Vector(transitionData["fromX"].get<float>(), transitionData["fromY"].get<float>());
            auto toVector = Vector(transitionData["toX"].get<float>(), transitionData["toY"].get<float>());
            newScene.addTransition(toSceneName, fromVector, toVector);
        }
        auto navigation = data.value("navigation", nlohmann::json::object());
        if (!navigation.empty()) {
            auto objectId = navigation.value("id", "");
            auto objPath = navigation["visual"]["obj"].get<std::string>();
            auto texturePath = navigation["visual"]["source"].get<std::string>();
            auto navMesh = std::make_shared<NavigationMesh>(m_window.getNavigationMesh(objPath.c_str(), texturePath.c_str()));
            if (!objectId.empty()) {
                navMesh->m_id = objectId;
            }
            navMesh->m_visual = loadVisualComponent(navigation["visual"]);
            newScene.setNavigationMesh(navMesh);
        }
        log("Scene loaded from file.");
        return newScene;
    }

    std::string Application::getTempDir()
    {
        return m_tempDir;
    }

    std::shared_ptr<VisualComponent> Application::getTexturedCube(const char* texturePath)
    {
        return m_window.getTexturedCube(texturePath);
    }

    bool Application::isPaused()
    {
        return m_paused;
    }

    void Application::loadSceneFrom(const char* path)
    {
        auto nextScene = getSceneFrom(path);
        setScene(nextScene);
    }

    std::shared_ptr<VisualComponent> Application::loadVisualComponent(nlohmann::json& data)
    {
        auto objFile = data.value("obj", "");
        std::shared_ptr<VisualComponent> visualPtr = nullptr;
        if (objFile.empty()) {
            visualPtr = getTexturedCube(data["source"].get<std::string>().c_str());
        } else {
            visualPtr = getTexturedMesh(objFile.c_str(), data["source"].get<std::string>().c_str());
        }
        visualPtr->move(Vector(data["posX"].get<float>(), data["posY"].get<float>()));
        visualPtr->scale(Vector3(data.value("scaleX", 1.f),
                            data.value("scaleY", 1.f),
                            data.value("scaleZ", 1.f)));
        visualPtr->setDebugOnly(data.value("debugOnly", false));
        return visualPtr;
    }

    void Application::log(const char* msg)
    {
        std::cout << "[Engine] " << msg << std::endl;
    }

    void Application::playAudio(const char* path, bool is3D, Vector3D pos)
    {
        m_audio.playAudio(path, is3D, pos);
    }

    void Application::run(const char* name, const Vector2D& windowSize)
    {
        log("PRE-START");
        std::chrono::time_point<std::chrono::steady_clock> startTime = std::chrono::steady_clock::now();
        m_audio.start();
        m_window.start(name, windowSize, &m_input);
        userStart();
        log("START");
        while (!m_window.shouldClose()) {
            float timeStep = 0.0;
            std::chrono::time_point<std::chrono::steady_clock> newTime = std::chrono::steady_clock::now();
            const auto frameTime = newTime - startTime;
            startTime = newTime;
            if (!isPaused()) {
                timeStep = std::chrono::duration_cast<std::chrono::duration<float>>(frameTime).count();
            }
            m_input.update(&m_scene, timeStep);
            m_window.update(&m_scene, isPaused());
            m_audio.update(&m_scene, timeStep);
            m_scene.update();
            doTransition();
        }
        log("PRE-END");
        m_audio.clear();
        m_scene.end();
        if (m_tempDir != nullptr) {
            std::filesystem::remove_all(Grafica::getPath(m_tempDir));
            log("Temporary directory cleared.");
        }
        m_audio.end();
        log("END");
    }

    void Application::run(const char* name, float windowSizeX, float windowSizeY)
    {
        run(name, Vector(windowSizeX, windowSizeY));
    }

    void Application::saveGameObject(GameObject* object, const char* path)
    {
        if (object == nullptr) {
            log("Can't save null game object.");
            return;
        }
        std::ofstream file;
        file.open(path);
        file << object->getData();
        file.close();
    }

    void Application::saveScene()
    {
        if (m_scene.m_name.empty()) {
            return;
        }
        if (m_tempDir == nullptr) {
            log("Can't save current scene without temporary directory.");
            return;
        }
        auto data = m_scene.getData();
        auto objects = data["objects"];
        for (auto objectData : objects) {
            auto objectId = objectData["id"].get<std::string>();
            auto object = m_scene.getById(objectId);
            if (object != nullptr) {
                auto input = object->getInputComponent();
                if (input != nullptr) {
                    m_input.m_loadedComponents[objectId] = input;
                }
            }
        }
        std::string fileName = "/" + m_scene.m_name;
        std::ofstream file;
        std::filesystem::create_directories(Grafica::getPath(m_tempDir));
        auto path = Grafica::getPath(m_tempDir + fileName);
        file.open(path);
        file << std::setw(4) << data;
        file.close();
    }

    void Application::setGlobalInput(std::shared_ptr<InputComponent>& inputComponent)
    {
        m_input.m_globalInput = inputComponent;
    }

    void Application::setPaused(bool value)
    {
        m_paused = value;
    }

    void Application::setNavigationMesh(Scene* scene, const char* objPath, const char* texturePath)
    {
        auto navMesh = std::make_shared<NavigationMesh>(m_window.getNavigationMesh(objPath, texturePath));
        navMesh->getVisualComponent()->setDebugOnly(true);
        m_scene.setNavigationMesh(navMesh);
    }

    void Application::setScene(Scene& scene)
    {
        log("Ending previous scene.");
        saveScene();
        m_audio.stopAll(&m_scene);
        m_scene.end();
        m_scene = scene;
        log("Previous scene ended.");
    }

    void Application::setTempDir(const char* path)
    {
        m_tempDir = const_cast<char*>(path);
        if (m_tempDir != nullptr) {
            std::filesystem::remove_all(Grafica::getPath(m_tempDir));
        }
    }
}