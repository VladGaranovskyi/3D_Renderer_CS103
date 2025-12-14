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
#include "UIcode.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

using namespace std;


int main(int argc, char** argv) {
    const int WIDTH = 1000;
    const int HEIGHT = 800;

    MainRenderer renderer(WIDTH, HEIGHT);
    Camera camera(WIDTH, HEIGHT);

    Mesh cubeMesh = MakeCubeMesh();
    Model model(cubeMesh, Transform());

    UIState ui;
    AppMode mode = AppMode::Settings;

    // Set default values
    ApplyUIToScene(ui, model, camera);
    ui.modelLoaded = true;

    // Start ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(renderer.GetWindow(), renderer.GetRenderer());
    ImGui_ImplSDLRenderer2_Init(renderer.GetRenderer());

    bool running = true;
    SDL_Event e;

    float yawSpeed = 1.2f;   // radians/sec
    float zoomSpeed = 2.0f;  // scale multiplier speed

    Uint64 lastCounter = SDL_GetPerformanceCounter();
    const double freq = (double)SDL_GetPerformanceFrequency();
    model.BuildTriangles(renderer, camera);

    while (running)
    {
        // Timing
        Uint64 now = SDL_GetPerformanceCounter();
        double dt = (now - lastCounter) / freq;
        lastCounter = now;

        // Events
        while (SDL_PollEvent(&e))
        {
            ImGui_ImplSDL2_ProcessEvent(&e);

            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) running = false;

        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Draw UI depending on mode
        if (mode == AppMode::Settings)
        {
            DrawSettingsUI(ui, mode, model, camera);
        }
        else
        {
            DrawViewerUI(ui, mode);

            // Apply zoom slider's value
            if(abs(camera.scaleZ - ui.camScaleZ) > 0.01f){
                camera.scaleZ = ui.camScaleZ;
                model.BuildTriangles(renderer, camera);
            }

            // Get keybord input
            const Uint8* keys = SDL_GetKeyboardState(nullptr);

            // Rotate y 
            if (keys[SDL_SCANCODE_LEFT])  {
                model.transform.rotation.y -= (float)(yawSpeed * dt);
                model.BuildTriangles(renderer, camera);
            }
            if (keys[SDL_SCANCODE_RIGHT]) {
                model.transform.rotation.y += (float)(yawSpeed * dt);
                model.BuildTriangles(renderer, camera);
            }

            // Rotate x
            if (keys[SDL_SCANCODE_UP])    {
                model.transform.rotation.x -= (float)(yawSpeed * dt);
                model.BuildTriangles(renderer, camera);
            }
            if (keys[SDL_SCANCODE_DOWN])  {
                model.transform.rotation.x += (float)(yawSpeed * dt);
                model.BuildTriangles(renderer, camera);
            }
        }

        // Render scene
        renderer.Clear();

        if (mode == AppMode::Viewer)
        {
            // Draw every frame, recalculate triangles only when changed
            model.DrawFilled(renderer);
        }

        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer.GetRenderer());

        renderer.ShowModel();
    }

    // ImGui shutdown
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    return 0;
    // Tasks
    /*
    -Mouse controls for rotating and arrow controls for moving
    -ray cast and path finding(implement the selection of points)
    -Camera controls like in Unity
    -clean up code
    -file upload (Saad)
    */
}

// Citations:
/*
https://en.wikipedia.org/wiki/3D_projection#Perspective_projection
https://wiki.libsdl.org/SDL2/CategoryRender
https://en.wikipedia.org/wiki/Rotation_matrix
https://www.youtube.com/watch?v=kdRJgYO1BJM
https://learnopengl.com/Getting-Started/Coordinate-Systems
https://github.com/ocornut/imgui?tab=readme-ov-file#getting-started--integration
https://www.gabrielgambetta.com/computer-graphics-from-scratch/07-filled-triangles.html
https://medium.com/nerd-for-tech/optimizing-3d-rendering-with-backface-culling-5430e0821e0a

*/
