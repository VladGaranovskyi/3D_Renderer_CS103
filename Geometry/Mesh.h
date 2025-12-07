#include "Vector3.h"
#include "Triangle.h"
#include <vector>

class Mesh{
    public:
        std::vector<Vector3> vertices;
        std::vector<Triangle> triangles;
};