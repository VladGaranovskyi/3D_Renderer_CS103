#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

#include "Management/MainRenderer.h"
#include "Management/UserQuery.h"
#include "Management/ObjLoader.h"

#include "Geometry/Model.h"
#include "Geometry/Mesh.h"
#include "Geometry/Transform.h"
#include "Geometry/Vector3.h"
#include "Geometry/Camera.h"

#include "UIcode.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_sdlrenderer2.h"

using namespace std;

int main(int argc, char** argv)
{
    const int WIDTH = 1000;
    const int HEIGHT = 800;

    cout << "3D Renderer" << endl;
    string objPath = chooseOBJ();

    if (objPath == "") {
        cout << "No file selected. Exiting..." << endl;
        return 0;
    }

    cout << "Loading: " << objPath << endl;

    Mesh loadedMesh;
    if (!LoadOBJFile(objPath, loadedMesh) || loadedMesh.vertices.empty()) {
        cout << "Failed to load OBJ. Using default cube." << endl;
        loadedMesh = MakeCubeMesh();
        objPath = "Objects/cube.obj";
    }

    MainRenderer renderer(WIDTH, HEIGHT);
    Camera camera(WIDTH, HEIGHT);

    Model model(loadedMesh, Transform());

    UIState ui;
    AppMode mode = AppMode::Settings;

    strcpy(ui.objFilePath, objPath.c_str());
    ApplyUIToScene(ui, model, camera);
    ui.modelLoaded = true;
    strcpy(ui.status, "Model loaded successfully");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(renderer.GetWindow(), renderer.GetRenderer());
    ImGui_ImplSDLRenderer2_Init(renderer.GetRenderer());

    bool running = true;
    SDL_Event e;

    float yawSpeed = 1.2f;
    Uint64 lastCounter = SDL_GetPerformanceCounter();
    const double freq = (double)SDL_GetPerformanceFrequency();

    model.BuildTriangles(renderer, camera);

    while (running)
    {
        Uint64 now = SDL_GetPerformanceCounter();
        double dt = (now - lastCounter) / freq;
        lastCounter = now;

        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL2_ProcessEvent(&e);

            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                running = false;
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

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

        
        // rebuild triangles when viewing
        if (mode == AppMode::Viewer) {
            model.BuildTriangles(renderer, camera);
        }

        renderer.Clear();

        if (mode == AppMode::Viewer) {
            model.DrawFilled(renderer);
        }

        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(
            ImGui::GetDrawData(),
            renderer.GetRenderer()
        );

        renderer.ShowModel();
    }

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
