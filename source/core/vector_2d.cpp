#include "vector_2d.hpp"

namespace QDUEngine
{
    Vector2D Vector(float x, float y)
    {
        return Vector2D{x, y};
    }

    Vector2D Vector(std::string& str)
    {
        std::vector<float> preVector;
        float sign = 1;
        for (auto c : str) {
            if (c == '-') {
                sign = -1;
            } else if (std::isdigit(c)) {
                preVector.push_back(sign * (float)(c - '0'));
                sign = 1;
            } else {
                sign = 1;
            }
        }
        return Vector2D{preVector[0], preVector[1]};
    }

    Vector2D Vector(const std::string& str)
    {
        std::string copy = str;
        return Vector(copy);
    }

    std::ostream &operator<<(std::ostream& os, const Vector2D& vector2D)
    {
        return os << "( " << vector2D.x << ", " << vector2D.y << ")";
    }

    bool operator==(const Vector2D& lhs, const Vector2D& rhs)
    {
        return lhs.x == rhs.x && lhs.y == rhs.y;
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