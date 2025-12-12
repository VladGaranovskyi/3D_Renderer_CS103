#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

class Vector3 {
public:
    float x;
    float y;
    float z;

    Vector3();
    Vector3(float x, float y, float z);

    Vector3 AddVector(const Vector3& vec) const;
    Vector3 SubtractVector(const Vector3& vec) const;
    Vector3 MultiplyVector(float m) const;
    float DotVector(const Vector3& vec) const;
    Vector3 CrossVector(const Vector3& vec) const;
    Vector3 GetNormalized() const;
};

#endif
