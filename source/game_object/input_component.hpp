#pragma once
#include <cstring>
#include <iostream>
#include "../core/vector_2d.hpp"

namespace QDUEngine
{
    class GameObject;
    class Scene;
    class InputComponent {
    public:
        InputComponent();
        virtual void onAction(Scene* scene, const char* action, float value) = 0;
        virtual void onCursorAction(const char* action, Vector2D& pos) = 0;
        virtual void onUpdate(float timeStep) = 0;
        static bool compare(const char* lhs, const char* rhs);
        void setGameObject(std::shared_ptr<GameObject>& gameObject);
    protected:
        std::shared_ptr<GameObject> m_gameObject = nullptr;
    };
}