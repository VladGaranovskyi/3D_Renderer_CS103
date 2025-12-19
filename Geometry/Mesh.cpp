#include "Mesh.h"
#include <vector>

using namespace std;

Mesh::Mesh(){
    vector<Vector3> v(8);
    vector<Triangle> t(24);
    vector<ScreenTriangle> s(24);
    vertices = v;
    triangles = t;
    screenTriangles = s;
}

Mesh::Mesh(vector<Vector3> v, vector<Triangle> t){
    vector<ScreenTriangle> s(t.size());
    vertices = v;
    triangles = t;
    screenTriangles = s;
}

void Mesh::ComputeMeshCenter(){
    Vector3 center;
    if (vertices.empty()) {
        meshCenter = center;
        return;
    }
    for(int i = 0; i < vertices.size(); i++){
        center.x += vertices.at(i).x;
        center.y += vertices.at(i).y;
        center.z += vertices.at(i).z;
    }
    meshCenter = center.MultiplyVector(1.0f / vertices.size());
}

void Mesh::HighlightTriangle(Vector2 screenPoint){
    int bestMatchIdx = -1;
    float bestZDepth = 1000000000;
    for(int i = 0; i < screenTriangles.size(); i++){
        ScreenTriangle t = screenTriangles.at(i);
        if(t.IsPointInTriangle(screenPoint)){
            float z = t.GetAverageDepth();
            if(z < bestZDepth){
                bestZDepth = z;
                bestMatchIdx = t.triangleIndex;
            }
        }
    }

    highlightedTriangleIdx = bestMatchIdx;
}