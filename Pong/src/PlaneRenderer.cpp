//============================
//Nagygyörgy Bence 11.b - 2015
//============================
#include "PlaneRenderer.h"

#define GLEW_STATIC

#include <glew.h>
#include<Pong3D.h>

int PlaneRenderer::_stride = 0;
unsigned int PlaneRenderer::_vbo = 0;
unsigned int PlaneRenderer::_ibo = 0;
unsigned int PlaneRenderer::_vao = 0;

void PlaneRenderer::Init()
{
    float hs = 1.0f;

    //POS NRM
    //XYZ XYZ = 8
    _stride = sizeof(float) * 6;

    //2D négyzet:
    //0,0
    //1,0
    //1,1
    //0,1
    float data[] = {
        -hs, 0, -hs, 0, 1, 0,
         hs, 0, -hs, 0, 1, 0,
         hs, 0,  hs, 0, 1, 0,
        -hs, 0,  hs, 0, 1, 0,
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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short) * (sizeof(idata)/sizeof(*idata)), idata, GL_STATIC_DRAW);

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    delete [] data;
    delete [] idata;
}

void PlaneRenderer::Cleanup()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ibo);
}

void PlaneRenderer::BindVAO()
{
    glBindVertexArray(_vao);
}

void PlaneRenderer::UnbindVAO()
{
    glBindVertexArray(0);
}
