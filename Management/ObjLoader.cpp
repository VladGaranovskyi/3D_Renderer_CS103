#include "ObjLoader.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

OBJ::OBJ(const string& path) {
    filePath = path;
    ifstream MyReadFile(path);
    string line;

    while (getline(MyReadFile, line)) {
        stringstream ss(line);
        string type;
        ss >> type;
 
        if(type == "v") {
            float m, n, p;
            ss >> m >> n >> p;
            vertices.push_back({m, n, p});
        }

        if(type == "f") {
            vector<int> face;
            int index;
            while (ss >> index) {
                face.push_back(index - 1);
            }
            faces.push_back(face);
        }
    }

    // create edges for wireframe
    for (int j = 0; j < faces.size(); j++) {
        vector<int> f = faces[j];
        for (int i = 0; i < f.size(); i++) {
            int a = f[i];
            int b = f[(i + 1) % f.size()];
            edges.push_back({a, b});
        }
    }
}

// fan triangulation: convert polygon to triangles
Mesh OBJ::ToMesh() {
    Mesh mesh;
    mesh.vertices = vertices;
    
    // triangulate each face using fan method
    for (int i = 0; i < faces.size(); i++) {
        vector<int> face = faces[i];
        
        if (face.size() < 3) continue;
        
        // fan triangulation: connect first vertex to all others
        for (int j = 1; j < face.size() - 1; j++) {
            Triangle tri(face[0], face[j], face[j + 1]);
            mesh.triangles.push_back(tri);
        }
    }
    
    mesh.ComputeMeshCenter();
    return mesh;
}

// load obj file directly into mesh
bool LoadOBJFile(const string& path, Mesh& outMesh) {
    OBJ obj(path);
    if (obj.vertices.empty()) {
        return false;
    }
    outMesh = obj.ToMesh();
    return true;
}
