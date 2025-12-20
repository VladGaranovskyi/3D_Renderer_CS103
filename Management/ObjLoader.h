#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "Edge.h"
#include "../Geometry/Mesh.h"
#include "../Geometry/Vector3.h"
#include "../Geometry/Triangle.h"

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>

using namespace std;

class OBJ {
public:
    vector<Vector3> vertices;
    vector<Edge> edges;
    vector<vector<int>> faces;
    string filePath;

    OBJ(const string& path);
    
    // convert to mesh with triangulated faces
    Mesh ToMesh();
};

// load obj file and return mesh with triangulated faces
bool LoadOBJFile(const string& path, Mesh& outMesh);

// "Upload" helper for ImGui: if inputPath is outside Objects/, copy it into Objects/ safely
// (and create a backup if overwriting), then return the path to load.
// Returns false if the source file doesn't exist or copy fails.
bool PrepareObjPathForLoading(const string& inputPath, string& outPath);

#endif
