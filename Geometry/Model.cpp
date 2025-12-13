#include "Model.h"
#include <vector>

using namespace std;

Model::Model(const Mesh& m, const Transform& t){
    mesh = m;
    transform = t;
}

void Model::Draw(MainRenderer& renderer, const Camera& camera){
    for(int i = 0; i < mesh.triangles.size(); i++){

        // Get coords
        Vector3 vertex1 = mesh.vertices.at(mesh.triangles.at(i).point1);
        Vector3 vertex2 = mesh.vertices.at(mesh.triangles.at(i).point2);
        Vector3 vertex3 = mesh.vertices.at(mesh.triangles.at(i).point3);

        // Convert to world
        Vector3 v1World = transform.LocalToWorld(&vertex1, mesh.meshCenter);
        Vector3 v2World = transform.LocalToWorld(&vertex2, mesh.meshCenter);
        Vector3 v3World = transform.LocalToWorld(&vertex3, mesh.meshCenter);

        // Get projection
        Vector2 screenPoint1 = camera.ProjectOnScreen(v1World);
        Vector2 screenPoint2 = camera.ProjectOnScreen(v2World);
        Vector2 screenPoint3 = camera.ProjectOnScreen(v3World);

        // Skip drawing if it is behind camera
        if((screenPoint1.x == -1 && screenPoint1.y == -1) || 
        (screenPoint2.x == -1 && screenPoint2.y == -1) || 
        (screenPoint3.x == -1 && screenPoint3.y == -1)){
            continue;
        }

        // Draw triangle
        renderer.DrawLine(screenPoint1.x, screenPoint1.y, screenPoint2.x, screenPoint2.y);
        renderer.DrawLine(screenPoint2.x, screenPoint2.y, screenPoint3.x, screenPoint3.y);
        renderer.DrawLine(screenPoint3.x, screenPoint3.y, screenPoint1.x, screenPoint1.y);
    }
}