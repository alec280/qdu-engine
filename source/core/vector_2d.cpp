#include "vector_2d.hpp"

namespace QDUEngine
{
    Vector2D Vector(float x, float y)
    {
        return Vector2D{x, y};
    }

    std::ostream &operator<<(std::ostream& os, const Vector2D& vector2D)
    {
        return os << "( " << vector2D.x << ", " << vector2D.y << ")";
    }

    Vector2D operator+(const Vector2D& lhs, const Vector2D& rhs)
    {
        return {lhs.x + rhs.x, lhs.y + rhs.y};
    }

    Vector2D& Vector2D::operator+=(const Vector2D& vector2D)
    {
        this->x += vector2D.x;
        this->y += vector2D.y;
        return *this;
    }

    Vector2D operator-(const Vector2D& lhs, const Vector2D& rhs)
    {
        return {lhs.x - rhs.x, lhs.y - rhs.y};
    }

    Vector2D& Vector2D::operator-=(const Vector2D& vector2D)
    {
        this->x -= vector2D.x;
        this->y -= vector2D.y;
        return *this;
    }

    Vector2D operator*(const Vector2D& lhs, float rhs)
    {
        return {lhs.x * rhs, lhs.y * rhs};
    }

    Vector2D& Vector2D::operator*=(float rhs)
    {
        this->x *= rhs;
        this->y *= rhs;
        return *this;
    }

    Vector2D operator*(float lhs, const Vector2D& rhs)
    {
        return {lhs * rhs.x, lhs * rhs.y};
    }
}