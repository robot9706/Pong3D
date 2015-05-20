//============================
//Nagygyörgy Bence 11.b - 2015
//============================
#include "UVPlane.h"

#include <glew.h>
#include<Pong3D.h>
#include <iostream>
using namespace std;

int UVPlane::_stride = 0;
unsigned int UVPlane::_vbo = 0;
unsigned int UVPlane::_ibo = 0;
unsigned int UVPlane::_vao = 0;

void UVPlane::Init()
{
    float hs = 1.0f;

    //POS NRM TS
    //XYZ XYZ XY = 8
    _stride = sizeof(float) * 8;

    //2D négyzet:
    //0,0
    //1,0
    //1,1
    //0,1
    float data[] = {
        -hs, 0, -hs, 0, 1, 0, 0, 0,
         hs, 0, -hs, 0, 1, 0, 1, 0,
         hs, 0,  hs, 0, 1, 0, 1, 1,
        -hs, 0,  hs, 0, 1, 0, 0, 1
    };

    short idata[] = { 1, 0, 2, 3, 2, 0 };

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

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, _stride, (void*)(6 * sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short) * (sizeof(idata)/sizeof(*idata)), idata, GL_STATIC_DRAW);

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    delete [] data;
}

void UVPlane::Cleanup()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ibo);
}

void UVPlane::BindVAO()
{
    glBindVertexArray(_vao);
}

void UVPlane::UnbindVAO()
{
    glBindVertexArray(0);
}
