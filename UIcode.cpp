#include "UIcode.h"

Mesh MakeCubeMesh()
{
    Mesh mesh;

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

    mesh.triangles = {
        //(back)
        Triangle{0, 3, 2}, Triangle{0, 2, 1},

        //(front)
        Triangle{4, 5, 6}, Triangle{4, 6, 7},

        //(left)
        Triangle{0, 4, 7}, Triangle{0, 7, 3},

        //(right)
        Triangle{1, 2, 6}, Triangle{1, 6, 5},

        //(bottom)
        Triangle{0, 1, 5}, Triangle{0, 5, 4},

        //(top)
        Triangle{3, 7, 6}, Triangle{3, 6, 2},
    };

    mesh.ComputeMeshCenter();

    return mesh;
}

// Sets values given in UI to the objects
void ApplyUIToScene(const UIState& ui, Model& model, Camera& camera)
{
    model.transform.position = Vector3{ ui.objPosition[0], ui.objPosition[1], ui.objPosition[2] };
    model.transform.rotation = Vector3{ ui.objRotation[0], ui.objRotation[1], ui.objRotation[2] };
    model.transform.scale = ui.objScale;

    camera.position = Vector3{ ui.camPosition[0], ui.camPosition[1], ui.camPosition[2] };
    camera.rotation = Vector3{ ui.camRotation[0], ui.camRotation[1], ui.camRotation[2] };
    camera.scaleZ = ui.camScaleZ;

    model.isCull = ui.cull;
    model.isFlip = ui.flip;
}


bool LoadMeshStub(const char* path, Mesh& outMesh)
{
    string pathStr(path);
    return LoadOBJFile(pathStr, outMesh);
}

// UI for settings
void DrawSettingsUI(UIState& ui, AppMode& mode, Model& model, Camera& camera)
{
    ImGui::Begin("Setup");

    ImGui::TextWrapped("Enter, apply, and view");
    ImGui::Separator();

    ImGui::InputText(".Obj file path", ui.objFilePath, IM_ARRAYSIZE(ui.objFilePath));
    ImGui::SameLine();
    if (ImGui::Button("Load"))
    {
        Mesh loaded;
        ui.loadFailed = false;

        bool ok = LoadMeshStub(ui.objFilePath, loaded);

        if (ok && !loaded.vertices.empty())
        {
            model.mesh = loaded;
            ui.modelLoaded = true;
            strcpy(ui.status, "Model loaded successfully");
        }
        else
        {
            ui.modelLoaded = false;
            ui.loadFailed = true;
            strcpy(ui.status, "Failed to load model");
        }
    }

    ImGui::TextColored(ui.loadFailed ? ImVec4(1,0.4f,0.4f,1) : ImVec4(0.7f,0.9f,0.7f,1),
                       "%s", ui.status);

    ImGui::Separator();

    ImGui::Checkbox("Enable culling", &ui.cull);
    ImGui::Checkbox("Flip normals", &ui.flip);
    ImGui::InputFloat("Camera Rotation Speed", &ui.mouseCameraRotationSpeed);
    ImGui::InputFloat("Object Rotation Speed", &ui.mouseObjectRotationSpeed);
    ImGui::InputFloat("Camera Movement Speed", &ui.cameraMovementSpeed);
    ImGui::InputFloat("Object Movement Speed", &ui.objectMovementSpeed);

    ImGui::Text("Object Transform");
    ImGui::InputFloat3("Obj Position", ui.objPosition);
    ImGui::InputFloat3("Obj Rotation (in radians)", ui.objRotation);
    ImGui::InputFloat("Obj Scale", &ui.objScale);

    ImGui::Separator();

    ImGui::Text("Camera Transform");
    ImGui::InputFloat3("Cam Position", ui.camPosition);
    ImGui::InputFloat3("Cam Rotation (in radians)", ui.camRotation);
    ImGui::SliderFloat("Zoom", &ui.camScaleZ, 0.5f, 30.0f);

    ImGui::Separator();
    
    if (ImGui::Button("Apply"))
    {
        ApplyUIToScene(ui, model, camera);
    }
    ImGui::SameLine();

    // Start Viewing if ready
    if (!ui.modelLoaded) ImGui::BeginDisabled();
    if (ImGui::Button("Start Viewer"))
    {
        ApplyUIToScene(ui, model, camera);
        mode = AppMode::Viewer;
    }
    if (!ui.modelLoaded) ImGui::EndDisabled();

    ImGui::End();
}

void DrawViewerAndInspectorSwitch(UIState& ui, AppMode& mode)
{
    int selectedMode = (mode == AppMode::Viewer) ? 0 : 1;

    ImGui::RadioButton("Viewer", &selectedMode, 0);
    ImGui::RadioButton("Inspector", &selectedMode, 1);

    mode = (selectedMode == 0) ? AppMode::Viewer : AppMode::Inspector;
}

// UI for viewing the 3d object
void DrawViewerUI(UIState& ui, AppMode& mode)
{
    ImGui::Begin("Viewer");
    DrawViewerAndInspectorSwitch(ui, mode);
    ImGui::Text("Controls:");
    ImGui::BulletText("Arrows: Move the object in xy plane");
    ImGui::BulletText("+/-: Move the object on the z-axis");
    ImGui::BulletText("Left Mouse Button: Hold LMB and move the mouse to rotate the object");
    ImGui::BulletText("Right Mouse Button: Hold RMB and move the mouse to rotate the camera");
    ImGui::BulletText("WASD: Hold RMB and Hold W/A/S/D to move the camera");
    ImGui::Separator();
    ImGui::SliderFloat("Zoom", &ui.camScaleZ, 0.5f, 30.0f);
    ImGui::InputFloat("Camera Rotation Speed", &ui.mouseCameraRotationSpeed);
    ImGui::InputFloat("Object Rotation Speed", &ui.mouseObjectRotationSpeed);
    ImGui::InputFloat("Camera Movement Speed", &ui.cameraMovementSpeed);
    ImGui::InputFloat("Object Movement Speed", &ui.objectMovementSpeed);

    if (ImGui::Button("Go Back"))
    {
        mode = AppMode::Settings;
    }

    ImGui::End();
}

void DrawInspectorUI(UIState& ui, AppMode& mode, Model& model, Camera& camera)
{
    ImGui::Begin("Inspector");
    DrawViewerAndInspectorSwitch(ui, mode);
    ImGui::Text("Controls:");
    ImGui::BulletText("Left Mouse Button: Touch the triangle you want to inspect");
    ImGui::BulletText("Right Mouse Button: Hold RMB and move the mouse to rotate the object");
    ImGui::Separator();

    ImGui::BeginDisabled(); 
    float position[3] = {model.transform.position.x, model.transform.position.y, model.transform.position.z};
    float rotation[3] = {model.transform.rotation.x, model.transform.rotation.y, model.transform.rotation.z};
    ImGui::InputFloat3("Transform Position", position, "%.3f");
    ImGui::InputFloat3("Transform Rotation", rotation, "%.3f");

    Vector3* verticesCoords = model.GetHighlightedVerticesWorld();
    if(verticesCoords != nullptr){
        float vertex1[3] = {verticesCoords[0].x, verticesCoords[0].y, verticesCoords[0].z};
        float vertex2[3] = {verticesCoords[1].x, verticesCoords[1].y, verticesCoords[1].z};
        float vertex3[3] = {verticesCoords[2].x, verticesCoords[2].y, verticesCoords[2].z};

        Vector3 normal = model.GetHighlightedNormal(verticesCoords);
        float normalFloat[3] = {normal.x, normal.y, normal.z};

        float* zDepths = model.GetHighlightedDepth(verticesCoords, camera);

        ImGui::InputInt("Triangle Index", &model.mesh.highlightedTriangleIdx);

        ImGui::InputFloat3("Vertex 1", vertex1, "%.3f");
        ImGui::InputFloat("Z Depth 1", &zDepths[0]);
        ImGui::InputFloat3("Vertex 2", vertex2, "%.3f");
        ImGui::InputFloat("Z Depth 1", &zDepths[1]);
        ImGui::InputFloat3("Vertex 3", vertex3, "%.3f");
        ImGui::InputFloat("Z Depth 1", &zDepths[2]);

        ImGui::InputFloat3("Normal", normalFloat, "%.3f");

        delete[] zDepths;
    }
    ImGui::EndDisabled();

    if (ImGui::Button("Go Back"))
    {
        mode = AppMode::Settings;
    }

    delete[] verticesCoords;

    ImGui::End();
}