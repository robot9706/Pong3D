//============================
//Nagygyörgy Bence 11.b - 2015
//============================
#include "SphereRenderer.h"

#include <glew.h>
#include<Pong3D.h>
#include <cmath>

#include <iostream>
using namespace std;

int SphereRenderer::_stride = 0;
unsigned int SphereRenderer::_vbo = 0;
unsigned int SphereRenderer::_ibo = 0;
unsigned int SphereRenderer::_vao = 0;
int SphereRenderer::IndexLength = 0;

void SphereRenderer::Init()
{
    float hs = 1.0f;

    //POS NRM
    //XYZ XYZ = 6
    _stride = sizeof(float) * 6;

    float radius = 1;
    int slices = 15;
    int stacks = 15;

    float data[(slices + 1) * (stacks + 1) * 6];

    float phi = 0.0f;
    float dphi = (float)M_PI / stacks;
    float dtheta = (M_PI * 2) / slices;
    float halfPi = (float)M_PI / 2.0f;

    float x,y,z,sc,theta;

    int index = 0;

    for(int s = 0;s<=stacks;s++)
    {
        phi = halfPi - s * dphi;

        y = radius * (float)sin(phi);
        sc = -radius * (float)cos(phi);

        for(int sl = 0;sl<=slices;sl++)
        {
            theta = sl * dtheta;
            x = sc * (float)sin(theta);
            z = sc * (float)cos(theta);

            //Pos
            data[index++] = x;
            data[index++] = y;
            data[index++] = z;

            //Normal
            data[index++] = x;
            data[index++] = y;
            data[index++] = z;
        }
    }

    IndexLength = slices * stacks * 6;
    short idata[IndexLength];

    index = 0;
    int k = slices + 1;

    for(int s = 0;s<=stacks;s++)
    {
        for(int sl = 0;sl<=slices;sl++)
        {
            idata[index++] = (short)((s + 0) * k + sl);
            idata[index++] = (short)((s + 1) * k + sl);
            idata[index++] = (short)((s + 0) * k + sl + 1);

            idata[index++] = (short)((s + 0) * k + sl + 1);
            idata[index++] = (short)((s + 1) * k + sl);
            idata[index++] = (short)((s + 1) * k + sl + 1);
        }
    }

    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ibo);

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (sizeof(data)/sizeof(*data)), data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, _stride, (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, _stride, (void*)(3 * sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short) * (sizeof(idata)/sizeof(*idata)), idata, GL_STATIC_DRAW);

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    delete [] data;
}

void SphereRenderer::Cleanup()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ibo);
}

void SphereRenderer::BindVAO()
{
    glBindVertexArray(_vao);
}

void SphereRenderer::UnbindVAO()
{
    glBindVertexArray(0);
}
