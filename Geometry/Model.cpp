#include "Model.h"
#include <vector>
#include <cmath>

using namespace std;

/*The following helper function (InterpolateX) was written with the aid of ChatGPT;  
  when prompted '(*uploaded the screenshot from the website with the algorithm of dra
wing triangles in pixels) can you make a c                                           ode
  for the InterpolateX method in the context of this webpage? Please give me only cod
e and minimal explanation'                                                             generated text:
  Helper: Interpolate(y0,x0,y1,x1) ΓåÆ vector of x values

    This returns an array x_values for each integer y between y0..y1 inclusive.      

    *The code is below*

  */

// Helper to get all x values between 2 x coords
static vector<int> InterpolateX(int y0, int x0, int y1, int x1)
{
    vector<int> listOfX;

    // if a straight line
    if (y0 == y1) {
        listOfX.push_back((int)round(x0));
        return listOfX;
    }

    int dy = y1 - y0;
    float dx = (float)(x1 - x0) / (float)dy;

    float x = (float)x0;

    // Assume y1 > y0
    for (int y = y0; y <= y1; y++) {
        listOfX.push_back((int)round(x));
        x += dx;
    }

    return listOfX;
}

static void RemoveLastValue(vector<int>& v)
{
    if (!v.empty()) v.pop_back();
}

static vector<int> ConcatVectors(const vector<int>& a, const vector<int>& b)
{
    vector<int> out;
    out.reserve(a.size() + b.size());

    for (int i = 0; i < a.size(); i++) out.push_back(a[i]);
    for (int i = 0; i < b.size(); i++) out.push_back(b[i]);

    return out;
}

Model::Model(){
    mesh = Mesh();
    transform = Transform();
}

Model::Model(const Mesh& m, const Transform& t){
    mesh = m;
    transform = t;
}

void Model::BuildTriangles(MainRenderer& renderer, const Camera& camera){
    mesh.screenTriangles.clear();
    for(int i = 0; i < mesh.triangles.size(); i++){

        // Get coords
        Vector3 vertex1 = mesh.vertices.at(mesh.triangles.at(i).point1);
        Vector3 vertex2 = mesh.vertices.at(mesh.triangles.at(i).point2);
        Vector3 vertex3 = mesh.vertices.at(mesh.triangles.at(i).point3);

        // Convert to world
        Vector3 v1World = transform.LocalToWorld(&vertex1, mesh.meshCenter);
        Vector3 v2World = transform.LocalToWorld(&vertex2, mesh.meshCenter);
        Vector3 v3World = transform.LocalToWorld(&vertex3, mesh.meshCenter);

        
        // Backface culling
        if(isCull){
            Vector3 v12 = v2World.SubtractVector(v1World);
            Vector3 v13 = v3World.SubtractVector(v1World);

            Vector3 triangleNormal = v12.CrossVector(v13);
            float dot = triangleNormal.DotVector(camera.position);
            if(isFlip){
                if (dot <= 0) continue;
            }
            else{
                if (dot >= 0) continue;
            }
        }
        

        // Get projection
        Vector3 screenPoint1 = camera.ProjectOnScreen(v1World);
        Vector3 screenPoint2 = camera.ProjectOnScreen(v2World);
        Vector3 screenPoint3 = camera.ProjectOnScreen(v3World);

        // Skip drawing if it is behind camera
        if((screenPoint1.x == -1 && screenPoint1.y == -1) || 
        (screenPoint2.x == -1 && screenPoint2.y == -1) || 
        (screenPoint3.x == -1 && screenPoint3.y == -1)){
            continue;
        }

        ScreenTriangle s(screenPoint1, screenPoint2, screenPoint3);
        // Set the original local triangle index
        s.triangleIndex = i;
        // If highlighted, set to true
        s.isHighlighted = i == mesh.highlightedTriangleIdx;
        mesh.screenTriangles.push_back(s);
    }
}

void Model::DrawEdges(MainRenderer& renderer){
    renderer.SetColor(0, 255, 0);
    for(int i = 0; i < mesh.screenTriangles.size(); i++){
        Vector2 screenPoint1 = mesh.screenTriangles[i].point1;
        Vector2 screenPoint2 = mesh.screenTriangles[i].point2;
        Vector2 screenPoint3 = mesh.screenTriangles[i].point3;
        renderer.DrawLine(screenPoint1.x, screenPoint1.y, screenPoint2.x, screenPoint2.y);
        renderer.DrawLine(screenPoint2.x, screenPoint2.y, screenPoint3.x, screenPoint3.y);
        renderer.DrawLine(screenPoint3.x, screenPoint3.y, screenPoint1.x, screenPoint1.y);
    }
}
void Model::DrawFilled(MainRenderer& renderer){
    for(int i = 0; i < mesh.screenTriangles.size(); i++){
        if(mesh.screenTriangles[i].isHighlighted) renderer.SetColor(255, 0, 0);
        else renderer.SetColor(0, 0, 255);
        Vector2 screenPoint1 = mesh.screenTriangles[i].point1;
        Vector2 screenPoint2 = mesh.screenTriangles[i].point2;
        Vector2 screenPoint3 = mesh.screenTriangles[i].point3;

        /*ScreenTriangle's constructor automatically sorts points by y coord*/

        if (screenPoint1.y == screenPoint3.y) continue;
        
        // Interpolation of all x coords between 2 points
        vector<int> x12 = InterpolateX(screenPoint1.y, screenPoint1.x, screenPoint2.y, screenPoint2.x);
        vector<int> x23 = InterpolateX(screenPoint2.y, screenPoint2.x, screenPoint3.y, screenPoint3.x);
        vector<int> x13 = InterpolateX(screenPoint1.y, screenPoint1.x, screenPoint3.y, screenPoint3.x);

        // Concatenate short sides
        RemoveLastValue(x12);
        vector<int> x123 = ConcatVectors(x12, x23);

        // What is left and what is right
        int m = (int)(x123.size() / 2);

        const vector<int>* xLeft  = nullptr;
        const vector<int>* xRight = nullptr;

        // Another check
        if (m >= 0 && m < (int)x13.size() && m < (int)x123.size() && x13[m] < x123[m]) {
            xLeft  = &x13;
            xRight = &x123;
        } else {
            xLeft  = &x123;
            xRight = &x13;
        }

        // Draw
        for (int y = screenPoint1.y; y <= screenPoint3.y; y++)
        {
            int idx = y - screenPoint1.y;
            if (idx < 0) continue;
            if (idx >= (int)xLeft->size())  continue;
            if (idx >= (int)xRight->size()) continue;

            int xStart = (*xLeft).at(idx);
            int xEnd   = (*xRight).at(idx);

            renderer.DrawHorizontalLine(xStart, xEnd, y);
        }
    }
    DrawEdges(renderer);
}

Vector3* Model::GetHighlightedVerticesWorld(){
    if(mesh.highlightedTriangleIdx == -1){
        return nullptr;
    }

    Vector3* out = new Vector3[3];
    out[0] = transform.LocalToWorld(mesh.vertices.at(mesh.triangles.at(mesh.highlightedTriangleIdx).point1));
    out[1] = transform.LocalToWorld(mesh.vertices.at(mesh.triangles.at(mesh.highlightedTriangleIdx).point2));
    out[2] = transform.LocalToWorld(mesh.vertices.at(mesh.triangles.at(mesh.highlightedTriangleIdx).point3));

    return out;
}

Vector3 Model::GetHighlightedNormal(Vector3* verticesWorld){
    if(verticesWorld == nullptr){
        return Vector3();
    }

    Vector3 AB = verticesWorld[1].SubtractVector(verticesWorld[0]);
    Vector3 AC = verticesWorld[2].SubtractVector(verticesWorld[0]);
    return AB.CrossVector(AC).GetNormalized();
}

float* Model::GetHighlightedDepth(Vector3* verticesWorld, Camera& camera){
    if(mesh.highlightedTriangleIdx == -1){
        return nullptr;
    }

    float* out = new float[3];
    out[0] = camera.WorldToCamera(verticesWorld[0]).z;
    out[1] = camera.WorldToCamera(verticesWorld[1]).z;
    out[2] = camera.WorldToCamera(verticesWorld[2]).z;

    return out;
}