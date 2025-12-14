#include "Camera.h"
#include <cmath>

Camera::Camera(int screenWidth, int screenHeight){
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->scaleZ = 5.0f;
    this->scaleX = 0.0f;
    this->scaleY = 0.0f;
}

Vector3 Camera::WorldToCamera(const Vector3& worldPoint) const{
    Vector3 p = worldPoint;
    p = p.SubtractVector(position); // P.x - C.x

    float cosTheta, sinTheta, x, y, z;

    // Inverse rotation
    // Rz rotation
    cosTheta = cos(-rotation.z);
    sinTheta = sin(-rotation.z);
    x = p.x * cosTheta - p.y * sinTheta;
    y = p.x * sinTheta + p.y * cosTheta;
    p.x = x;
    p.y = y;
    
    // Ry rotation
    cosTheta = cos(-rotation.y);
    sinTheta = sin(-rotation.y);
    x = p.x * cosTheta + p.z * sinTheta;
    z = -(p.x * sinTheta) + p.z * cosTheta;
    p.x = x;
    p.z = z;

    // Rx rotation
    cosTheta = cos(-rotation.x);
    sinTheta = sin(-rotation.x);
    y = p.y * cosTheta - p.z * sinTheta;
    z = p.y * sinTheta + p.z * cosTheta;
    p.y = y;
    p.z = z;

    return p;
}

Vector2 Camera::CameraToScreen(const Vector3& cameraPoint) const{
    Vector2 p;
    if(cameraPoint.z <= 0){
        p.x = -1;
        p.y = -1;
        return p;
    }

    p.x = (cameraPoint.x * scaleZ / cameraPoint.z) + scaleX;
    p.y = (cameraPoint.y * scaleZ / cameraPoint.z) + scaleY;

    return p;
}

Vector3 Camera::ProjectOnScreen(const Vector3& worldPoint) const{
    Vector3 p3 = WorldToCamera(worldPoint);
    Vector2 screenPoint;
    
    screenPoint = CameraToScreen(p3);
    if(screenPoint.x == -1 && screenPoint.y == -1){
        return Vector3(-1, -1, 0);
    }

    screenPoint.x = screenWidth * 0.5f + screenPoint.x * screenWidth * 0.2f;
    screenPoint.y = screenHeight * 0.5f - screenPoint.y * screenHeight * 0.2f;
    

    return Vector3(screenPoint.x, screenPoint.y, p3.z);
}

void Camera::Zoom(float incr){
    scaleZ += incr;
    if (scaleZ < 0.1f) scaleZ = 0.1f;
}