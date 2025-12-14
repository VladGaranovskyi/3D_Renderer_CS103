#include "Transform.h"
#include "Vector3.h"
#include <cmath>

using namespace std;

Transform::Transform(Vector3 p, Vector3 r, float s){
    position = p;
    rotation = r;
    scale = s;
}

Transform::Transform(){
    position = Vector3(0, 0, 0);
    rotation = Vector3(0, 0, 0);
    scale = 1;
}

Vector3 Transform::LocalToWorld(const Vector3* localPoint, const Vector3& centerPoint) const{
    Vector3 p = *localPoint;
    p = p.SubtractVector(centerPoint);
    p.MultiplyVector(scale);

    float cosTheta, sinTheta, x, y, z;
    
    // Rx rotation
    cosTheta = cos(rotation.x);
    sinTheta = sin(rotation.x);
    y = p.y * cosTheta - p.z * sinTheta;
    z = p.y * sinTheta + p.z * cosTheta;
    p.y = y;
    p.z = z;

    // Ry rotation
    cosTheta = cos(rotation.y);
    sinTheta = sin(rotation.y);
    x = p.x * cosTheta + p.z * sinTheta;
    z = -(p.x * sinTheta) + p.z * cosTheta;
    p.x = x;
    p.z = z;

    // Rz rotation
    cosTheta = cos(rotation.z);
    sinTheta = sin(rotation.z);
    x = p.x * cosTheta - p.y * sinTheta;
    y = p.x * sinTheta + p.y * cosTheta;
    p.x = x;
    p.y = y;
    
    return p.AddVector(position);
}