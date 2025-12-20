#ifndef MESH_H
#define MESH_H

#include "Vector3.h"
#include "Triangle.h"
#include "ScreenTriangle.h"
#include "Transform.h"
#include <vector>

class Mesh{
    public:
        // Local center
        Vector3 meshCenter;
        std::vector<Vector3> vertices;
        std::vector<Triangle> triangles;
        // List of triangles in pixel coordinates
        std::vector<ScreenTriangle> screenTriangles;
        int highlightedTriangleIdx = -1; 

        Mesh();
        Mesh(std::vector<Vector3> v, std::vector<Triangle> t);

        void ComputeMeshCenter();
        // Finds the best matching triangle when user in inspector mode clicks LMB
        void HighlightTriangle(Vector2 screenPoint);
};
#endif