//============================
//Nagygyörgy Bence 11.b - 2015
//============================
#include "Matrix4.h"

#include <cmath>

void Matrix4::SetIdentityMatrix(float* mx)
{
    for (int i = 0; i < 16; ++i)
        mx[i] = 0.0f;

    for (int i = 0; i < 4; ++i)
        mx[i + i * 4] = 1.0f;
}

//Forrás: OpenTK
void Matrix4::CreateOrthographicOffCenter(float* mx, float left, float right, float bottom, float top, float near, float far)
{
    float invRL = 1.0f / (right - left);
    float invTB = 1.0f / (top - bottom);
    float invFN = 1.0f / (far - near);

    mx[0] = 2 * invRL;
    mx[5] = 2 * invTB;
    mx[10] = -2 * invFN;
    mx[12] = -(right + left) * invRL;
    mx[13] = -(top + bottom) * invTB;
    mx[14] = -(far + near) * invFN;
}

void Matrix4::CreateTranslation(float* mx, float x, float y, float z)
{
    float tmp[16];
    SetIdentityMatrix(tmp);

    tmp[12] = x;
    tmp[13] = y;
    tmp[14] = z;

    Multiply(mx, tmp);
}

void Matrix4::CreateScale(float* mx, float x, float y, float z)
{
    float tmp[16];
    SetIdentityMatrix(tmp);

    tmp[0] = x;
    tmp[5] = y;
    tmp[10] = z;

    Multiply(mx, tmp);
}

void Matrix4::CreateRotationX(float* mx, float rad)
{
    float cosv = cos(rad);
    float sinv = sin(rad);

    mx[5] *= cosv;
    mx[6] *= sinv;
    mx[9] *= -sinv;
    mx[10] *= cosv;
}

void Matrix4::CreateRotationY(float* mx, float rad)
{
    float cosv = cos(rad);
    float sinv = sin(rad);

    mx[0] *= cosv;
    mx[3] *= -sinv;
    mx[8] *= sinv;
    mx[10] *= cosv;
}

void Matrix4::CreateRotationZ(float* mx, float rad)
{
    float cosv = cos(rad);
    float sinv = sin(rad);

    mx[0] *= cosv;
    mx[1] *= sinv;
    mx[4] *= -sinv;
    mx[5] *= cosv;
}

void Matrix4::Multiply(float* a, float* b)
{
    float res[16];

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            res[j*4 + i] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                res[j*4 + i] += a[k*4 + i] * b[j*4 + k];
            }
        }
    }

    for(int x = 0;x<16;x++){
        a[x] = res[x];
    }
}
