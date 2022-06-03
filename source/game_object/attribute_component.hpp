#pragma once
#include "../core/dictionary.hpp"

namespace QDUEngine
{
    class GameObject;
    class AttributeComponent {
    public:
        explicit AttributeComponent(char* name);
        virtual ~AttributeComponent() = default;
        //virtual void update(GameObject& obj) = 0;
        void set(char* key, float value);
        float get(char* key);
        char* getName();
    protected:
        char* m_name{};
        Dictionary<char*, float> m_attr{};
    };
}