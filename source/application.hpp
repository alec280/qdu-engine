#pragma once
#include "scene/scene.hpp"

namespace QDUEngine
{
    class Application {
    public:
        void run(const char *name, const Vector2D& windowSize, Scene& scene);
        void bindKey(const char* key, const char* action);
    private:
        Scene* m_scene{};
        std::map<const char*, const char*> m_keyBindings{};
    };
}
