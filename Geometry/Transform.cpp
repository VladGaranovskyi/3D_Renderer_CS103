#include "Transform.h"
#include <cmath>

Transform::Transform(): position(), rotation(), scale(1.0f) {}
Transform::Transform(const Vector3& p, const Vector3& r, float s): position(p), rotation(r), scale(s) {}
Vector3 Transform::LocalToWorld(const Vector3& localPoint) const {
    Vector3 p = localPoint.MultiplyVector(scale);
    float cx = std::cos(rotation.x);
    float sx = std::sin(rotation.x);
    float cy = std::cos(rotation.y);
    float sy = std::sin(rotation.y);
    float cz = std::cos(rotation.z);
    float sz = std::sin(rotation.z);
    float x = p.x;
    float y = p.y;
    float z = p.z;
    float nx = x;
    float ny = y * cx - z * sx;
    float nz = y * sx + z * cx;
    x = nx; y = ny; z = nz;
    nx = x * cy + z * sy;
    nz = -x * sy + z * cy;
    x = nx; z = nz;
    nx = x * cz - y * sz;
    ny = x * sz + y * cz;
    x = nx; y = ny;
    x += position.x;
    y += position.y;
    z += position.z;
    return Vector3(x,y,z);
}

Vector3 Transform::LocalToWorld(const Vector3* localPoint, const Vector3& center) const {
    Vector3 relative = localPoint->SubtractVector(center);
    Vector3 scaled = relative.MultiplyVector(scale);
    
    float cx = std::cos(rotation.x);
    float sx = std::sin(rotation.x);
    float cy = std::cos(rotation.y);
    float sy = std::sin(rotation.y);
    float cz = std::cos(rotation.z);
    float sz = std::sin(rotation.z);
    
    float x = scaled.x;
    float y = scaled.y;
    float z = scaled.z;
    
    // rotate x
    float nx = x;
    float ny = y * cx - z * sx;
    float nz = y * sx + z * cx;
    x = nx; y = ny; z = nz;
    
    // rotate y
    nx = x * cy + z * sy;
    nz = -x * sy + z * cy;
    x = nx; z = nz;
    
    // rotate z
    nx = x * cz - y * sz;
    ny = x * sz + y * cz;
    x = nx; y = ny;
    
    x += position.x + center.x;
    y += position.y + center.y;
    z += position.z + center.z;
    return Vector3(x, y, z);
}