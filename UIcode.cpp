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
}


bool LoadMeshStub(const char* path, Mesh& outMesh)
{
    outMesh = MakeCubeMesh();
    return true;
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

        // Objloader when done
        bool ok = LoadMeshStub(ui.objFilePath, loaded);

        if (ok)
        {
            model.mesh = loaded;
            ui.modelLoaded = true;
        }
        else
        {
            ui.modelLoaded = false;
            ui.loadFailed = true;
        }
    }

    ImGui::TextColored(ui.loadFailed ? ImVec4(1,0.4f,0.4f,1) : ImVec4(0.7f,0.9f,0.7f,1),
                       "%s", ui.status);

    ImGui::Separator();

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

// UI for viewing the 3d object
void DrawViewerUI(UIState& ui, AppMode& mode)
{
    ImGui::Begin("Viewer");
    ImGui::Text("Controls:");
    ImGui::BulletText("Arrows: Rotate object");
    ImGui::BulletText("Backspace: Return to the settings");
    ImGui::Separator();
    ImGui::SliderFloat("Zoom", &ui.camScaleZ, 0.5f, 30.0f);

    if (ImGui::Button("Go Back"))
    {
        mode = AppMode::Settings;
    }

    ImGui::End();
}