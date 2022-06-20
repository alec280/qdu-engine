#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "vector_2d.hpp"

namespace QDUEngine
{
    struct Vector3D
    {
        float x, y, z;
        Vector3D& operator+=(const Vector3D& vector3D);
        Vector3D& operator-=(const Vector3D& vector3D);
        Vector3D& operator+=(const Vector2D& vector2D);
        Vector3D& operator-=(const Vector2D& vector2D);
        Vector3D& operator*=(float rhs);
        std::string toString() const;
    };

    Vector3D Vector3(float x, float y, float z);
    Vector3D Vector3(std::string& str);
    Vector3D Vector3(const std::string& str);
    std::ostream& operator<<(std::ostream& os, const Vector3D& vector3D);
    bool operator==(const Vector3D& lhs, const Vector3D& rhs);
    Vector3D operator+(const Vector3D& lhs, const Vector3D& rhs);
    Vector3D operator+(const Vector3D& lhs, const Vector2D& rhs);
    Vector3D operator-(const Vector3D& lhs, const Vector3D& rhs);
    Vector3D operator-(const Vector3D& lhs, const Vector2D& rhs);
    Vector3D operator*(const Vector3D& lhs, float rhs);
    Vector3D operator*(float lhs, const Vector3D& rhs);
}