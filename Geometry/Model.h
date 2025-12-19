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

    void Draw(MainRenderer& renderer, const Camera& camera) const;
    void BuildTriangles(MainRenderer& renderer, const Camera& camera);
    void DrawFilled(MainRenderer& renderer);
    void DrawEdges(MainRenderer& renderer);
    Vector3* GetHighlightedVerticesWorld();
    Vector3 GetHighlightedNormal(Vector3* verticesWorld);
    float* GetHighlightedDepth(Vector3* verticesWorld, Camera& camera);
};

#endif
