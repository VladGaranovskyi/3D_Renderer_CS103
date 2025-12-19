#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include "Transform.h"
#include "Camera.h"
#include "../Management/MainRenderer.h"

class Model {
public:
    Mesh mesh;
    Transform transform;
    bool isCull = true;
    bool isFlip = false;

    Model();
    Model(const Mesh& m, const Transform& t);

    // Calculate all the geometry needed
    void BuildTriangles(MainRenderer& renderer, const Camera& camera);

    // Draw filled faces with edges
    void DrawFilled(MainRenderer& renderer);

    // Draw wireframe only
    void DrawEdges(MainRenderer& renderer);

    // Get world vertices of the highlighted triangle
    Vector3* GetHighlightedVerticesWorld();

    // Calculate normal of the highlighted triangle
    Vector3 GetHighlightedNormal(Vector3* verticesWorld);

    // Get z-depth of the highlighted triangle
    float* GetHighlightedDepth(Vector3* verticesWorld, Camera& camera);
};

#endif
