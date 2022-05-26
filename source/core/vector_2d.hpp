#pragma once
#include <iostream>
#include <vector>

namespace QDUEngine
{
    struct Vector2D
    {
        float x, y;
        Vector2D& operator+=(const Vector2D& vector2D);
        Vector2D& operator-=(const Vector2D& vector2D);
        Vector2D& operator*=(float rhs);
    };

    Vector2D Vector(float x, float y);
    Vector2D Vector(std::string& str);
    Vector2D Vector(const std::string& str);
    std::ostream& operator<<(std::ostream& os, const Vector2D& vector2D);
    bool operator==(const Vector2D& lhs, const Vector2D& rhs);
    Vector2D operator+(const Vector2D& lhs, const Vector2D& rhs);
    Vector2D operator-(const Vector2D& lhs, const Vector2D& rhs);
    Vector2D operator*(const Vector2D& lhs, float rhs);
    Vector2D operator*(float lhs, const Vector2D& rhs);
}