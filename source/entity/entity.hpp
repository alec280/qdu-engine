#pragma once
#include "../core/dictionary.hpp"

namespace QDUEngine
{
    class Entity {
    public:
        explicit Entity(char *mName);
        Vector2D& getPosition();
        char* getName();
        void move(const Vector2D& by);
    private:
        char* m_name;
        Vector2D m_position{};
        Dictionary<char*, int> m_attr = Dictionary<char *, int>(nullptr, 0);
    };

    std::ostream& operator<<(std::ostream& os, const Entity& entity);
}
