#pragma once
#include "../core/dictionary.hpp"

namespace QDUEngine
{
    class Entity {
    public:
        explicit Entity(char *mName);
        Vector2D& getPosition();
        void set(char* key, float value);
        float get(char* key);
        char* getName();
        void move(const Vector2D& by);
        virtual void update(float delta);
    private:
        char* m_name;
        Vector2D m_position{};
        Dictionary<char*, float> m_attr{};
    };

    std::ostream& operator<<(std::ostream& os, const Entity& entity);
}
