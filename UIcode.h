#ifndef UICODE_H
#define UICODE_H

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_sdlrenderer2.h"
#include <cmath>
#include "Management/MainRenderer.h"
#include "Management/ObjLoader.h"
#include "Geometry/Model.h"
#include "Geometry/Mesh.h"
#include "Geometry/Transform.h"
#include "Geometry/Vector3.h"
#include "Geometry/Camera.h"
#include "Geometry/Triangle.h"



Mesh MakeCubeMesh();

enum class AppMode {
    Settings,
    Viewer
};


class UIState {
    public:
        char objFilePath[500] = "Objects/cube.obj";

        float objPosition[3]   = {0, 0, 0};
        float objRotation[3]   = {0, 0, 0};   
        float objScale    = 5.0f;

        float camPosition[3]   = {0, 0, -20};
        float camRotation[3]   = {0, 0, 0};   
        float camScaleZ   = 5.0f;        

        bool modelLoaded  = false;
        bool loadFailed   = false;
        char status[256]  = "A model needs to be loaded";
        float mouseObjectRotationSpeed = 0.003f;
        float mouseCameraRotationSpeed = 0.004f;
        float objectMovementSpeed = 10.0f;
        float cameraMovementSpeed = 10.0f;
        bool cull = true;
        bool flip = false;
};

void ApplyUIToScene(const UIState& ui, Model& model, Camera& camera);
bool LoadMeshStub(const char* path, Mesh& outMesh);
void DrawSettingsUI(UIState& ui, AppMode& mode, Model& model, Camera& camera);
void DrawViewerUI(UIState& ui, AppMode& mode);


#endif