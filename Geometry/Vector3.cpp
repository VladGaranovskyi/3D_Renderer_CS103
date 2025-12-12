#include "Vector3.h"

Vector3::Vector3() : x(0), y(0), z(0) {}

Vector3::Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

Vector3 Vector3::AddVector(const Vector3& vec) const {
    return Vector3(x + vec.x, y + vec.y, z + vec.z);
}

Vector3 Vector3::SubtractVector(const Vector3& vec) const {
    return Vector3(x - vec.x, y - vec.y, z - vec.z);
}

Vector3 Vector3::MultiplyVector(float m) const {
    return Vector3(x * m, y * m, z * m);
}

float Vector3::DotVector(const Vector3& vec) const {
    return x * vec.x + y * vec.y + z * vec.z;
}

Vector3 Vector3::CrossVector(const Vector3& vec) const {
    return Vector3(
        y * vec.z - z * vec.y,
        z * vec.x - x * vec.z,
        x * vec.y - y * vec.x
    );
}

Vector3 Vector3::GetNormalized() const {
    float len = std::sqrt(x*x + y*y + z*z);
    if (len == 0) return Vector3(0, 0, 0);
    return Vector3(x/len, y/len, z/len);
}
