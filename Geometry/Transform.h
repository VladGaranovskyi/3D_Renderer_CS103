#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "Vector3.h"

class Transform {
public:
    // position in space
    Vector3 position;  

    // rotation by euler angles
    Vector3 rotation;  

    // scale of the object
    float scale;     

    Transform();
    Transform(Vector3 p, Vector3 r, float s);

    // Transform local coords to world(scale, rotate, shift)
    // Each vertex coordinate changes according to Transform
    Vector3 LocalToWorld(const Vector3* localPoint, const Vector3& centerPoint) const;
};
#endif