#ifndef MESH_H
#define MESH_H

#include "Vector3.h"
#include "Triangle.h"
#include "ScreenTriangle.h"
#include "Transform.h"
#include <vector>

class Mesh{
    public:
        Vector3 meshCenter;
        std::vector<Vector3> vertices;
        std::vector<Triangle> triangles;
        std::vector<ScreenTriangle> screenTriangles;
        int highlightedTriangleIdx = -1; 

        Mesh();
        Mesh(std::vector<Vector3> v, std::vector<Triangle> t);

        void ComputeMeshCenter();
        void HighlightTriangle(Vector2 screenPoint);
};
#endif