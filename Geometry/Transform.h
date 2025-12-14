#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vector3.h"

class Transform {
public:
    Vector3 position;
    Vector3 rotation;
    float scale;

    Transform();
    Transform(const Vector3& p, const Vector3& r, float s);

    Vector3 LocalToWorld(const Vector3& localPoint) const;
    Vector3 LocalToWorld(const Vector3* localPoint, const Vector3& center) const;
};

#endif
