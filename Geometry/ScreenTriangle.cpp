#include "ScreenTriangle.h"

ScreenTriangle::ScreenTriangle(){
    point1 = Vector2(0, 0);
    point2 = Vector2(0, 0);
    point3 = Vector2(0, 0);
    z1 = 0;
    z2 = 0;
    z3 = 0;
}

ScreenTriangle::ScreenTriangle(Vector3 p1, Vector3 p2, Vector3 p3){
    point1 = Vector2(p1.x, p1.y);
    point2 = Vector2(p2.x, p2.y);
    point3 = Vector2(p3.x, p3.y);
    z1 = p1.z;
    z2 = p2.z;
    z3 = p3.z;

    // Sorting
    if (point2.y < point1.y) {
        float tempZ = z1;
        Vector2 tempV = point1;
        z1 = z2;
        z2 = tempZ;
        point1 = point2;
        point2 = tempV;
    }
    if (point3.y < point1.y) {
        float tempZ = z1;
        Vector2 tempV = point1;
        z1 = z3;
        z3 = tempZ;
        point1 = point3;
        point3 = tempV;
    }
    if (point3.y < point2.y) {
        float tempZ = z2;
        Vector2 tempV = point2;
        z2 = z3;
        z3 = tempZ;
        point2 = point3;
        point3 = tempV;
    }
}