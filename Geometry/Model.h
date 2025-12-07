#include "Mesh.h"
#include "Transform.h"
#include "Camera.h"
#include "../Management/MainRenderer.h"

class Model {
public:
    Mesh mesh;
    Transform transform;

    Model(const Mesh& m, const Transform& t);

    void Draw(const MainRenderer& renderer, const Camera& camera);
};
