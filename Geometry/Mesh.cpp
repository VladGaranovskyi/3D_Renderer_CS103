#include "Mesh.h"
#include <vector>

using namespace std;

// Default Constructor
Mesh::Mesh(){
    vector<Vector3> v(8);
    vector<Triangle> t(24);
    vector<ScreenTriangle> s(24);
    vertices = v;
    triangles = t;
    screenTriangles = s;
}

// Constructor
Mesh::Mesh(vector<Vector3> v, vector<Triangle> t){
    vector<ScreenTriangle> s(t.size());
    vertices = v;
    triangles = t;
    screenTriangles = s;
}

// Get the local center of the 3d model for easier rotation
void Mesh::ComputeMeshCenter(){
    Vector3 center;
    if (vertices.empty()) {
        meshCenter = center;
        return;
    }
    
    // Sum all xyz coords to get average after
    for(int i = 0; i < vertices.size(); i++){
        center.x += vertices.at(i).x;
        center.y += vertices.at(i).y;
        center.z += vertices.at(i).z;
    }
    meshCenter = center.MultiplyVector(1.0f / vertices.size());
}

// Find the best matching triangle when user in inspector mode clicks LMB
void Mesh::HighlightTriangle(Vector2 screenPoint){
    int bestMatchIdx = -1;
    float bestZDepth = 1000000000;
    for(int i = 0; i < screenTriangles.size(); i++){
        ScreenTriangle t = screenTriangles.at(i);

        // check every ScreenTriangle
        if(t.IsPointInTriangle(screenPoint)){
            float z = t.GetAverageDepth();

            // The least depth is the closest to the camera
            if(z < bestZDepth){
                bestZDepth = z;
                bestMatchIdx = t.triangleIndex;
            }
        }
    }

    highlightedTriangleIdx = bestMatchIdx;
}