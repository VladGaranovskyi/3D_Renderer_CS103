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
    for(int i = 0; i < vertices.size(); i++){
        center.x += vertices.at(i).x;
        center.y += vertices.at(i).y;
        center.z += vertices.at(i).z;
    }
    center = center.MultiplyVector(1.0f / vertices.size());
}