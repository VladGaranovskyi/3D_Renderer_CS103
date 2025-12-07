#include "Vector3.h"
#include "Vector2.h"

class Camera{
    public:
        Vector3 position;
        Vector3 rotation;

        // Scales of x, y of the screen
        float scaleX = 1.0f;
        float scaleY = 1.0f;

        // Distance of the camera to the screen
        float scaleZ;

        int screenWidth;
        int screenHeight;

        Camera(int screenWidth, int screenHeight);

        // Convert the worldpoint relative to camera
        Vector3 WorldToCamera(const Vector3& worldPoint) const;

        // Convert abstract coordinates to pixels for SDL2
        Vector2 CameraToScreen(const Vector2& cameraPoint) const;

        // Helper method
        Vector2 ProjectOnScreen(const Vector2& worldPoint) const;

        // Change scale Z
        void Zoom(float incr);
};