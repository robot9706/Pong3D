#ifndef MATRIX4_H
#define MATRIX4_H


class Matrix4
{
    public:
        static void SetIdentityMatrix(float* mx);
        static void CreateOrthographicOffCenter(float* mx, float left, float right, float bottom, float top, float zNear, float zFar);
        static void CreateTranslation(float *mx, float x, float y, float z);
        static void CreateScale(float* mx, float x, float y, float z);
        static void CreateRotationX(float* mx, float rad);
        static void CreateRotationY(float* mx, float rad);
        static void CreateRotationZ(float* mx, float rad);
        static void Multiply(float* a, float* b);
};

#endif // MATRIX4_H
