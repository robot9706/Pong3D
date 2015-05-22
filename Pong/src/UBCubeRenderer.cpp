//============================
//Nagygyörgy Bence 11.b - 2015
//============================
#include "UBCubeRenderer.h"

#include <glew.h>
#include <Pong3D.h>
#include <iostream>
using namespace std;

int UBCubeRenderer::_stride = 0;
unsigned int UBCubeRenderer::_vbo = 0;
unsigned int UBCubeRenderer::_ibo = 0;
unsigned int UBCubeRenderer::_vao = 0;

void UBCubeRenderer::Init()
{
    //POS UV NRM
    //XYZ TS XYZ = 8
    _stride = sizeof(float) * 8;

    float data[] = {
        //Elsõ oldal
        -1.0f, -1.0f,  1.0f, 0, 0, 1, 0, 0,
         1.0f, -1.0f,  1.0f, 0, 0, 1, 0, 0,
         1.0f,  1.0f,  1.0f, 0, 0, 1, 0, 0,
        -1.0f,  1.0f,  1.0f, 0, 0, 1, 0, 0,

        //Felsõ oldal
        -1.0f,  1.0f,  1.0f, 0, 1, 0, 0, 1,
         1.0f,  1.0f,  1.0f, 0, 1, 0, 1, 1,
         1.0f,  1.0f, -1.0f, 0, 1, 0, 1, 0,
        -1.0f,  1.0f, -1.0f, 0, 1, 0, 0, 0,

        //Hátsó oldal
        -1.0f,  1.0f, -1.0f, 0, 0, -1, 0, 0,
         1.0f,  1.0f, -1.0f, 0, 0, -1, 0, 0,
         1.0f, -1.0f, -1.0f, 0, 0, -1, 0, 0,
        -1.0f, -1.0f, -1.0f, 0, 0, -1, 0, 0,

        //Bal oldal
        -1.0f, -1.0f, -1.0f, -1, 0, 0, 0, 0,
        -1.0f, -1.0f,  1.0f, -1, 0, 0, 0, 0,
        -1.0f,  1.0f,  1.0f, -1, 0, 0, 0, 0,
        -1.0f,  1.0f, -1.0f, -1, 0, 0, 0, 0,

        //Alsó oldal
        -1.0f, -1.0f,  1.0f, 0, -1, 0, 0, 0,
         1.0f, -1.0f,  1.0f, 0, -1, 0, 0, 0,
         1.0f, -1.0f, -1.0f, 0, -1, 0, 0, 0,
        -1.0f, -1.0f, -1.0f, 0, -1, 0, 0, 0,

        //Jobb oldal
         1.0f, -1.0f,  1.0f, 1, 0, 0, 0, 0,
         1.0f, -1.0f, -1.0f, 1, 0, 0, 0, 0,
         1.0f,  1.0f, -1.0f, 1, 0, 0, 0, 0,
         1.0f,  1.0f,  1.0f, 1, 0, 0, 0, 0,

    };

    short idata[] = {
            //elsõ
            0, 1, 2, 2, 3, 0,
            //tetõ
            4, 5, 6, 6, 7, 4,
            //hátsó
            8, 9, 10, 10, 11, 8,
            // bal
            12, 13, 14, 14, 15, 12,
            // alsó
            18, 17, 16, 16, 19, 18,
            // jobb
            20, 21, 22, 22, 23, 20, };

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

void UBCubeRenderer::Cleanup()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ibo);
}

void UBCubeRenderer::BindVAO()
{
    glBindVertexArray(_vao);
}

void UBCubeRenderer::UnbindVAO()
{
    glBindVertexArray(0);
}
