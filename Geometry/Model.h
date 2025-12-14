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

    Model();
    Model(const Mesh& m, const Transform& t);

    void Draw(MainRenderer& renderer, const Camera& camera) const;
    void BuildTriangles(MainRenderer& renderer, const Camera& camera);
    void DrawFilled(MainRenderer& renderer);
    void DrawEdges(MainRenderer& renderer);
};

#endif
