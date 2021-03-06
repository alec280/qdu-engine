#pragma once
#include <deque>
#include "scene/window.hpp"
#include "audio/audio.hpp"

namespace QDUEngine
{
    class Application {
    public:
        virtual void userStart() = 0;
        virtual void onTransition() = 0;
        void bindCursorButton(Input::CursorButton cursorButton, const char* action);
        void bindKey(const char* key, const char* action);
        void bindJoystick(const char* key, const char* action);
        GameObject getGameObjectFrom(const char* path);
        GameObject getGameObjectFrom(const char* path, std::shared_ptr<InputComponent>& input);
        Scene getSceneFrom(const char* path);
        static std::string getAbsolutePath(const char* path);
        std::shared_ptr<AudioComponent> getAudio(const char* audioPath);
        Vector3D getCameraPosition();
        std::shared_ptr<GameObject> getMainObject();
        std::shared_ptr<NavigationMesh> getNavigationMesh();
        std::shared_ptr<VisualComponent> getTexturedMesh(const char* objPath, const char* texturePath);
        std::string getTempDir();
        std::shared_ptr<VisualComponent> getTexturedCube(const char* texturePath);
        bool isPaused();
        void loadSceneFrom(const char* path);
        std::shared_ptr<VisualComponent> loadVisualComponent(nlohmann::json& data);
        void moveCamera(const Vector3D& by);
        void moveCamera(Vector3D& by);
        void playAudio(const char* path, bool is3D, Vector3D pos);
        void resetCamera();
        void run(const char* name, const Vector2D& windowSize);
        void run(const char *name, float windowSizeX, float windowSizeY);
        static void saveGameObject(GameObject* object, const char *path);
        void saveScene(const char* path);
        Vector3D screenToWorld(Vector2D& screenPos, Vector3D& plane, float depth);
        void setGlobalInput(std::shared_ptr<InputComponent>& inputComponent);
        void setPaused(bool value);
        void setNavigationMesh(Scene* scene, const char* objPath, const char* texturePath);
        void setScene(Scene& scene);
        void setTempDir(const char* path);
    protected:
        explicit Application();
        Scene m_scene{};
    private:
        Audio m_audio{};
        bool m_debugMode = false;
        DebugManager m_debug{};
        Input m_input{};
        bool m_mustResume = true;
        bool m_paused = false;
        std::deque<float> m_recentFrameRates{};
        char* m_tempDir = nullptr;
        Window m_window{};
        void doTransition();
        float getRunningAverage();
        static void log(const char* msg);
        void saveScene(const char* path, bool temp);
        void setDebugMode(bool value);
    };
}
