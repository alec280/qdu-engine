#include "entity.hpp"

namespace QDUEngine
{
    Entity::Entity(char* mName) : m_name(mName) {}

    std::ostream &operator<<(std::ostream& os, Entity& entity)
    {
        return os << entity.getName() << entity.getPosition();
    }

    Vector2D& Entity::getPosition()
    {
        return m_position;
    }

    char* Entity::getName()
    {
        return m_name;
    }

    void Entity::move(const Vector2D& by)
    {
        m_position += by;
    }

    void Entity::set(char *key, float value)
    {
        m_attr.set(key, value);
    }

    float Entity::get(char *key)
    {
        return m_attr.get(key);
    }

    void Entity::update(float delta) {}
}