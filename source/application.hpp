#pragma once
#include "scene/window.hpp"
#include "scene/scene.hpp"


namespace QDUEngine
{
    class Application {
    public:
        virtual void userStart() = 0;
        void bindCursorButton(Input::CursorButton cursorButton, const char* action);
        void bindKey(const char* key, const char* action);
        void bindJoystick(const char* key, const char* action);
        void loadSceneFrom(const char* path);
        GameObject getGameObjectFrom(const char* path);
        GameObject getGameObjectFrom(const char* path, std::shared_ptr<InputComponent>& input);
        Scene getSceneFrom(const char* path);
        static std::string getAbsolutePath(const char* path);
        std::string getTempDir();
        std::shared_ptr<VisualComponent> getTexturedCube(const char* texturePath);
        void run(const char* name, const Vector2D& windowSize);
        void run(const char *name, float windowSizeX, float windowSizeY);
        static void saveGameObject(GameObject* object, const char *path);
        void setGlobalInput(std::shared_ptr<InputComponent>& inputComponent);
        void setScene(Scene& scene);
        void setTempDir(const char* path);
    protected:
        explicit Application();
        Scene m_scene{};
    private:
        Input m_input{};
        char* m_tempDir = nullptr;
        Window m_window{};
        void doTransition();
        static void log(const char* msg);
        void saveScene();
    };
}
