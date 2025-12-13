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

    Model(const Mesh& m, const Transform& t);

    void Draw(MainRenderer& renderer, const Camera& camera);
};
#endif
