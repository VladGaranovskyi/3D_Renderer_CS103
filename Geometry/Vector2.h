#ifndef VECTOR2_H
#define VECTOR2_H

class Vector2 {
public:
    float x;
    float y;
    Vector2();
    Vector2(float x, float y);
    Vector2 AddVector(const Vector2& vec) const;
    Vector2 SubtractVector(const Vector2& vec) const;
    Vector2 MultiplyVector(float m) const;
};

#endif