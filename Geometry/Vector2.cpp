#include "Vector2.h"

Vector2::Vector2(): x(0), y(0) {}
Vector2::Vector2(float x_, float y_): x(x_), y(y_) {}
Vector2 Vector2::AddVector(const Vector2& vec) const {
    return Vector2(x + vec.x, y + vec.y);
}
Vector2 Vector2::SubtractVector(const Vector2& vec) const {
    return Vector2(x - vec.x, y - vec.y);
}
Vector2 Vector2::MultiplyVector(float m) const {
    return Vector2(x * m, y * m);
}

