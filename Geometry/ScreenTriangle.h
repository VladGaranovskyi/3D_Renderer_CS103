#ifndef SCREENTRIANGLE_H
#define SCREENTRIANGLE_H

#include "Vector2.h"
#include "Vector3.h"

class ScreenTriangle{
    public:
        Vector2 point1;
        Vector2 point2;
        Vector2 point3;
        int triangleIndex;
        bool isHighlighted;

        float z1;
        float z2;
        float z3;

        ScreenTriangle();
        ScreenTriangle(Vector3 p1, Vector3 p2, Vector3 p3);

        float GetAverageDepth() const;

        bool IsPointInTriangle(Vector2 screenPoint);
};

#endif