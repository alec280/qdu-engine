#pragma once
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
        void preloadJSON(const char* path);
        void run(const char* name, const Vector2D& windowSize);
        void run(const char *name, float x, float y);
        void setGlobalInput(std::shared_ptr<InputComponent>& inputComponent);
        void setTempDir(const char* path);
    protected:
        //Application();
        explicit Application(Scene* scene);
        //Scene* m_nextScene{};
        Scene* m_scene{};
    private:
        bool m_shouldClose = false;
        char* m_tempDir = nullptr;
        static void log(const char* msg);
    };
}
