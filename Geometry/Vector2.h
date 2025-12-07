
class Vector2{
    public:
        float x;
        float y;

        Vector2(float x, float y);

        Vector2 AddVector(Vector2 vec) const;
        Vector2 SubtractVector(Vector2 vec) const;
        Vector2 MultiplyVector(float m) const;

};