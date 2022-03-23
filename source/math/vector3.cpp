#pragma once
#include "vector3.h"

std::ostream& operator<<(std::ostream& os, const Vector3& v3)
{
    return os << "( " << v3.x << ", " << v3.y << ", " << v3.z << ")";
}

Vector3 Vector3::operator+(const Vector3& v3) const
{
    return { x + v3.x, y + v3.y, z + v3.z };
}

Vector3& Vector3::operator+=(const Vector3& v3)
{
    x += v3.x;
    y += v3.y;
    z += v3.z;
    return *this;
}

Vector3 Vector3::operator-(const Vector3& v3) const
{
    return { x - v3.x, y - v3.y, z - v3.z };
}

Vector3& Vector3::operator-=(const Vector3& v3)
{
    x -= v3.x;
    y -= v3.y;
    z -= v3.z;
    return *this;
}

Vector3 Vector3::operator*(const Vector3& v3) const
{
    return { x * v3.x, y * v3.y, z * v3.z };
}

Vector3& Vector3::operator*=(const Vector3& v3)
{
    x *= v3.x;
    y *= v3.y;
    z *= v3.z;
    return *this;
}

Vector3 Vector3::operator*(const float& f) const
{
    return { x * f, y * f, z * f };
}

Vector3& Vector3::operator*=(const float& f)
{
    x *= f;
    y *= f;
    z *= f;
    return *this;
}


Vector3 Vector3::operator/(const Vector3& v3) const
{
    return { x / v3.x, y / v3.y, z / v3.z };
}

Vector3& Vector3::operator/=(const Vector3& v3)
{
    x /= v3.x;
    y /= v3.y;
    z /= v3.z;
    return *this;
}

Vector3 Vector3::operator/(const float& f) const
{
    return { x / f, y / f, z / f };
}

Vector3& Vector3::operator/=(const float& f)
{
    x /= f;
    y /= f;
    z /= f;
    return *this;
}