class Vector3{
    public:
        float x;
        float y;
        float z;

        Vector3(float x, float y, float z);

        Vector3 AddVector(Vector3 vec) const;
        Vector3 SubtractVector(Vector3 vec) const;
        Vector3 MultiplyVector(float m) const;

        float DotVector(Vector3 vec) const;
        Vector3 CrossVector(Vector3 vec) const;
        Vector3 GetNormalized() const;

};