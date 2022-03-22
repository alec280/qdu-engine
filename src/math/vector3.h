#pragma once
#include <iostream>

struct Vector3
{
    float x, y, z;

    Vector3& operator+=(const Vector3& v3);
    Vector3 operator+(const Vector3& v3) const;

    Vector3& operator-=(const Vector3& v3);
    Vector3 operator-(const Vector3& v3) const;

    Vector3& operator*=(const Vector3& v3);
    Vector3 operator*(const Vector3& v3) const;
    Vector3& operator*=(const float& f);
    Vector3 operator*(const float& f) const;

    Vector3& operator/=(const Vector3& v3);
    Vector3 operator/(const Vector3& v3) const;
    Vector3& operator/=(const float& f);
    Vector3 operator/(const float& f) const;
};

std::ostream& operator<<(std::ostream& os, const Vector3& v3);
