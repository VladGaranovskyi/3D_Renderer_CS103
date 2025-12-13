#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
#include "Management/MainRenderer.h"
#include "Geometry/Model.h"
#include "Geometry/Mesh.h"
#include "Geometry/Transform.h"
#include "Geometry/Vector3.h"
#include "Geometry/Camera.h"
#include "Geometry/Triangle.h"

using namespace std;

Mesh MakeCubeMesh()
{
    Mesh mesh;

    // Cube centered at origin, side length = 2
    // (so vertices are at +/-1)
    mesh.vertices = {
        Vector3{-1, -1, -1}, // 0
        Vector3{ 1, -1, -1}, // 1
        Vector3{ 1,  1, -1}, // 2
        Vector3{-1,  1, -1}, // 3
        Vector3{-1, -1,  1}, // 4
        Vector3{ 1, -1,  1}, // 5
        Vector3{ 1,  1,  1}, // 6
        Vector3{-1,  1,  1}, // 7
    };

    // Triangulate each cube face (2 triangles per face = 12 triangles total)
    // Assuming your Mesh stores triangles/faces. Adjust field name if needed:
    // e.g. mesh.triangles, mesh.faces, mesh.Triangles, etc.
    mesh.triangles = {
        // back face (-Z): 0,1,2,3
        Triangle{0, 1, 2}, Triangle{0, 2, 3},
        // front face (+Z): 4,5,6,7
        Triangle{4, 6, 5}, Triangle{4, 7, 6},
        // left face (-X): 0,3,7,4
        Triangle{0, 3, 7}, Triangle{0, 7, 4},
        // right face (+X): 1,5,6,2
        Triangle{1, 6, 5}, Triangle{1, 2, 6},
        // bottom face (-Y): 0,4,5,1
        Triangle{0, 5, 4}, Triangle{0, 1, 5},
        // top face (+Y): 3,2,6,7
        Triangle{3, 6, 2}, Triangle{3, 7, 6},
    };

    mesh.ComputeMeshCenter();

    return mesh;
}


int main(int argc, char** argv) {
    const int WIDTH = 1000;
    const int HEIGHT = 800;

    // Initialize renderer
    MainRenderer renderer(WIDTH, HEIGHT);

    // 2) Initialize camera
    // Adjust to your Camera constructor.
    // Common: Camera(int screenW, int screenH)
    Camera camera(WIDTH, HEIGHT);

    // Put camera back so cube is in front of it.
    // Depending on your convention (+Z forward or -Z forward), you may need to flip sign.
    camera.position = Vector3{0, 0, -25};   // try {0,0,-5} first
    camera.rotation = Vector3{0, 0, 0};
    camera.scaleZ = 5.0f;                      // “focal length / projection plane distance” (tune later)

    // 3) Initialize mesh + model + transform
    Mesh cubeMesh = MakeCubeMesh();
    Model model(cubeMesh, Transform());
    model.transform.scale = 5.0f;

    // 4) Main loop
    bool running = true;
    SDL_Event e;

    float yawSpeed = 1.2f;     // radians/sec
    float zoomSpeed = 2.0f;    // units/sec (camera move)
    float camZ = camera.position.z;

    Uint64 lastCounter = SDL_GetPerformanceCounter();
    const double freq = (double)SDL_GetPerformanceFrequency();

    while (running)
    {
        // --- timing
        Uint64 now = SDL_GetPerformanceCounter();
        double dt = (now - lastCounter) / freq;
        lastCounter = now;

        // --- handle events
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) running = false;

            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
            }
        }

        // --- update (hold keys)
        const Uint8* keys = SDL_GetKeyboardState(nullptr);

        // Rotate cube (yaw around Y)
        if (keys[SDL_SCANCODE_LEFT])  model.transform.rotation.y -= (float)(yawSpeed * dt);
        if (keys[SDL_SCANCODE_RIGHT]) model.transform.rotation.y += (float)(yawSpeed * dt);

        // Optional pitch
        if (keys[SDL_SCANCODE_UP])    model.transform.rotation.x -= (float)(yawSpeed * dt);
        if (keys[SDL_SCANCODE_DOWN])  model.transform.rotation.x += (float)(yawSpeed * dt);

        // Zoom: move camera forward/back OR scale model (choose one)
        if (keys[SDL_SCANCODE_W]) camZ += (float)(zoomSpeed * dt); // closer (depending on convention)
        if (keys[SDL_SCANCODE_S]) camZ -= (float)(zoomSpeed * dt); // farther
        camera.position.z = camZ;

        // If you prefer scaling instead of moving camera:
        // if (keys[SDL_SCANCODE_W]) model.transform.scale *= (float)(1.0 + 1.0 * dt);
        // if (keys[SDL_SCANCODE_S]) model.transform.scale *= (float)(1.0 - 1.0 * dt);

        // --- render
        renderer.Clear();

        // Your Model::Draw should accept (MainRenderer&, Camera&) or similar.
        // Adjust signature as needed.
        model.Draw(renderer, camera);

        renderer.ShowModel();


        // Tasks
        /*
        -Selection of object transform, .obj filename
        -Slider for zoom
        -Mouse rotation of camera
        -WASD Translate Camera like in Unity
        -textfield for file upload
        -backface culling
        -draw edges
        -clean up code
        -find docs for code
        -ray cast and path finding(implement the selection of points)
        */

    }
    return 0;
}

// Citations:
/*
https://en.wikipedia.org/wiki/3D_projection#Perspective_projection
https://wiki.libsdl.org/SDL2/CategoryRender
https://en.wikipedia.org/wiki/Rotation_matrix
https://www.youtube.com/watch?v=kdRJgYO1BJM
https://learnopengl.com/Getting-Started/Coordinate-Systems

*/
