#include "../Geometry/Mesh.h"
#include <string>

class ObjLoader {
public:
    // load the 3D object into Mesh
    bool load(std::string filename, Mesh& outMesh);
};