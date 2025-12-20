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

    Uint64 lastCounter = SDL_GetPerformanceCounter();
    const double freq = (double)SDL_GetPerformanceFrequency();

    model.BuildTriangles(renderer, camera);
    bool isRecalculated = false, isLMBdown = false, isRMBdown = false, isLMBclicked = false;
    int mouseDx = 0, mouseDy = 0, mouseXPos = 0, mouseYPos = 0;

    while (running)
    {
        Uint64 now = SDL_GetPerformanceCounter();
        double dt = (now - lastCounter) / freq;
        lastCounter = now;
        mouseDx = 0;
        mouseDy = 0;

        // Events and input loop
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL2_ProcessEvent(&e);

            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                running = false;
            
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    isLMBdown = true;
                    isLMBclicked = true;
                }
                if (e.button.button == SDL_BUTTON_RIGHT) {
                    isRMBdown = true;
                    SDL_SetRelativeMouseMode(SDL_TRUE);
                }
            }

            if (e.type == SDL_MOUSEBUTTONUP) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    isLMBdown = false;
                    isLMBclicked = false;
                }
                if (e.button.button == SDL_BUTTON_RIGHT) {
                    isRMBdown = false;
                    SDL_SetRelativeMouseMode(SDL_FALSE);
                }
            }

            if (e.type == SDL_MOUSEMOTION) {
                mouseXPos = e.motion.x;
                mouseYPos = e.motion.y;
                if (isLMBdown || isRMBdown) {
                    mouseDx += e.motion.xrel;
                    mouseDy += e.motion.yrel;
                }
            }

            if (e.type == SDL_WINDOWEVENT)
{
                if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    int newW = e.window.data1;
                    int newH = e.window.data2;

                    renderer.Resize(newW, newH);     
                    camera.screenWidth = newW;
                    camera.screenHeight = newH;


                    model.BuildTriangles(renderer, camera);
                }
            }
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if (mode == AppMode::Settings)
        {
            DrawSettingsUI(ui, mode, model, camera);

            if (ui.requestRebuild)
            {
                model.BuildTriangles(renderer, camera);
                ui.requestRebuild = false;
            }
        }
        else if(mode == AppMode::Inspector){
            DrawInspectorUI(ui, mode, model, camera);

            if(isLMBclicked){
                model.mesh.HighlightTriangle(Vector2(mouseXPos, mouseYPos));
                isLMBclicked = false;
                isRecalculated = true;
            }

            if (isRMBdown) {
                model.transform.rotation.y -= mouseDx * ui.mouseObjectRotationSpeed;
                model.transform.rotation.x -= mouseDy * ui.mouseObjectRotationSpeed; 

                isRecalculated = true;
            }

            if(isRecalculated){
                model.BuildTriangles(renderer, camera);
                isRecalculated = false;
            }
        }
        else
        {
            DrawViewerUI(ui, mode);

            model.mesh.highlightedTriangleIdx = -1;

            // Apply zoom slider's value
            if(abs(camera.scaleZ - ui.camScaleZ) > 0.01f){
                camera.scaleZ = ui.camScaleZ;
                model.BuildTriangles(renderer, camera);
            }

            // Get keybord input
            const Uint8* keys = SDL_GetKeyboardState(nullptr);

            // Object Rotation
            if (isLMBdown) {
                model.transform.rotation.y -= mouseDx * ui.mouseObjectRotationSpeed;
                model.transform.rotation.x -= mouseDy * ui.mouseObjectRotationSpeed; 

                isRecalculated = true;
            }

            // Object Movement
            if (keys[SDL_SCANCODE_LEFT]){ 
                model.transform.position.x -= ui.objectMovementSpeed * (float)dt;
                isRecalculated = true; 
            }
            if (keys[SDL_SCANCODE_RIGHT]){ 
                model.transform.position.x += ui.objectMovementSpeed * (float)dt;
                isRecalculated = true; 
            }
            if (keys[SDL_SCANCODE_UP]){ 
                model.transform.position.y += ui.objectMovementSpeed * (float)dt; 
                isRecalculated = true; 
            }
            if (keys[SDL_SCANCODE_DOWN]){ 
                model.transform.position.y -= ui.objectMovementSpeed * (float)dt; 
                isRecalculated = true; 
            }

            if (keys[SDL_SCANCODE_KP_PLUS] || keys[SDL_SCANCODE_EQUALS]) { 
                model.transform.position.z += ui.objectMovementSpeed * (float)dt;
                isRecalculated = true;
            }
            if (keys[SDL_SCANCODE_KP_MINUS] || keys[SDL_SCANCODE_MINUS]) {
                model.transform.position.z -= ui.objectMovementSpeed * (float)dt;
                isRecalculated = true;
            }

            // Camera rotation and movement
            if (isRMBdown) {
                Vector3 move;

                if (keys[SDL_SCANCODE_W]) move = move.AddVector(camera.GetForward());
                if (keys[SDL_SCANCODE_S]) move = move.SubtractVector(camera.GetForward());
                if (keys[SDL_SCANCODE_D]) move = move.AddVector(camera.GetRight());
                if (keys[SDL_SCANCODE_A]) move = move.SubtractVector(camera.GetRight());

                if (keys[SDL_SCANCODE_E]) move.y += 1.0f;
                if (keys[SDL_SCANCODE_Q]) move.y -= 1.0f;

                camera.position = camera.position.AddVector(
                    move.MultiplyVector(ui.cameraMovementSpeed * (float)dt)
                );

                camera.rotation.y += mouseDx * ui.mouseCameraRotationSpeed;
                camera.rotation.x += mouseDy * ui.mouseCameraRotationSpeed; 

                isRecalculated = true;
            }

            if(isRecalculated){
                model.BuildTriangles(renderer, camera);
                isRecalculated = false;
            }
        }

    

        renderer.Clear();

        if (mode != AppMode::Settings) {
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

// Tasks
    /*
    -Convert the console file upload into the UI with checkboxes and all that stuff
    -file upload (Saad)
    */

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
https://gamedev.stackexchange.com/questions/190054/how-to-calculate-the-forward-up-right-vectors-using-the-rotation-angles#190058
https://www.baeldung.com/cs/check-if-point-is-in-2d-triangle
https://www.geeksforgeeks.org/cpp/enumeration-in-cpp/

https://www.geeksforgeeks.org/cpp/enumeration-in-cpp/

*/
