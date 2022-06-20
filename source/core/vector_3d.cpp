#include "vector_3d.hpp"

namespace QDUEngine
{
    Vector3D Vector3(float x, float y, float z)
    {
        return Vector3D{x, y, z};
    }

    Vector3D Vector3(std::string& str)
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
        return Vector3D{preVector[0], preVector[1], preVector[3]};
    }

    Vector3D Vector3(const std::string& str)
    {
        std::string copy = str;
        return Vector3(copy);
    }

    std::string Vector3D::toString() const
    {
        std::string result =
                "(" +
                std::to_string((int)x) +
                "," +
                std::to_string((int)y) +
                "," +
                std::to_string((int)z) +
                ")";
        return result;
    }

    std::ostream &operator<<(std::ostream& os, const Vector3D& vector3D)
    {
        return os << "( " << vector3D.x << ", " << vector3D.y << ", " << vector3D.z << ")";
    }

    bool operator==(const Vector3D& lhs, const Vector3D& rhs)
    {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }

    Vector3D operator+(const Vector3D& lhs, const Vector3D& rhs)
    {
        return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
    }

    Vector3D& Vector3D::operator+=(const Vector3D& vector3D)
    {
        this->x += vector3D.x;
        this->y += vector3D.y;
        this->z += vector3D.z;
        return *this;
    }

    Vector3D operator+(const Vector3D& lhs, const Vector2D& rhs)
    {
        return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z};
    }

    Vector3D& Vector3D::operator+=(const Vector2D& vector2D)
    {
        this->x += vector2D.x;
        this->y += vector2D.y;
        return *this;
    }

    Vector3D operator-(const Vector3D& lhs, const Vector3D& rhs)
    {
        return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
    }

    Vector3D& Vector3D::operator-=(const Vector3D& vector3D)
    {
        this->x -= vector3D.x;
        this->y -= vector3D.y;
        this->z -= vector3D.z;
        return *this;
    }

    Vector3D operator-(const Vector3D& lhs, const Vector2D& rhs)
    {
        return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z};
    }

    Vector3D& Vector3D::operator-=(const Vector2D& vector2D)
    {
        this->x -= vector2D.x;
        this->y -= vector2D.y;
        return *this;
    }

    Vector3D operator*(const Vector3D& lhs, float rhs)
    {
        return {lhs.x * rhs, lhs.y * rhs, lhs.z * rhs};
    }

    Vector3D& Vector3D::operator*=(float rhs)
    {
        this->x *= rhs;
        this->y *= rhs;
        this->z *= rhs;
        return *this;
    }

    Vector3D operator*(float lhs, const Vector3D& rhs)
    {
        return {lhs * rhs.x, lhs * rhs.y, lhs * rhs.z};
    }
}