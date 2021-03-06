//============================
//Nagygy�rgy Bence 11.b - 2015
//============================
#include "BB.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

BB::BB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, int tag)
{
    Tag = tag;

    MinX = (maxX < minX ? maxX : minX);
    MinY = (maxY < minY ? maxY : minY);
    MinZ = (maxZ < minZ ? maxZ : minZ);
    MaxX = (maxX > minX ? maxX : minX);
    MaxY = (maxY > minY ? maxY : minY);
    MaxZ = (maxZ > minZ ? maxZ : minZ);

    SizeX = (MaxX - MinX) / 2.0f;
    SizeY = (MaxY - MinY) / 2.0f;
    SizeZ = (MaxZ - MinZ) / 2.0f;

    CenterX = (MaxX + MinX) / 2.0f;
    CenterY = (MaxY + MinY) / 2.0f;
    CenterZ = (MaxZ + MinZ) / 2.0f;
}

BB::BB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
    Tag = 0;

    MinX = (maxX < minX ? maxX : minX);
    MinY = (maxY < minY ? maxY : minY);
    MinZ = (maxZ < minZ ? maxZ : minZ);
    MaxX = (maxX > minX ? maxX : minX);
    MaxY = (maxY > minY ? maxY : minY);
    MaxZ = (maxZ > minZ ? maxZ : minZ);

    SizeX = (MaxX - MinX) / 2.0f;
    SizeY = (MaxY - MinY) / 2.0f;
    SizeZ = (MaxZ - MinZ) / 2.0f;

    CenterX = (MaxX + MinX) / 2.0f;
    CenterY = (MaxY + MinY) / 2.0f;
    CenterZ = (MaxZ + MinZ) / 2.0f;
}

BB::BB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, int tag, float sc)
{
    Tag = tag;
    AnimScale = sc;

    MinX = (maxX < minX ? maxX : minX);
    MinY = (maxY < minY ? maxY : minY);
    MinZ = (maxZ < minZ ? maxZ : minZ);
    MaxX = (maxX > minX ? maxX : minX);
    MaxY = (maxY > minY ? maxY : minY);
    MaxZ = (maxZ > minZ ? maxZ : minZ);

    SizeX = (MaxX - MinX) / 2.0f;
    SizeY = (MaxY - MinY) / 2.0f;
    SizeZ = (MaxZ - MinZ) / 2.0f;

    CenterX = (MaxX + MinX) / 2.0f;
    CenterY = (MaxY + MinY) / 2.0f;
    CenterZ = (MaxZ + MinZ) / 2.0f;
}

glm::mat4 BB::GetWorldMatrix()
{
    glm::mat4 w = glm::translate(glm::mat4(1.0f), glm::vec3(CenterX, CenterY, CenterZ));
    w = glm::scale(w, glm::vec3(SizeX, SizeY, SizeZ));

    return w;
}

glm::mat4 BB::GetWorldMatrixPlane()
{
    glm::mat4 w = glm::translate(glm::mat4(1.0f), glm::vec3(CenterX, CenterY, CenterZ));
    w = glm::scale(w, glm::vec3(SizeX, 1, SizeZ));

    return w;
}

bool BB::Insecrets(float x, float z, BB* o, float ox, float oz)
{
    float left = CenterX - SizeX + x;
    float right = CenterX + SizeX + x;
    float top = CenterZ - SizeZ + z;
    float bottom = CenterZ + SizeZ + z;

    float bleft = o->CenterX - o->SizeX + ox;
    float bright = o->CenterX + o->SizeX + ox;
    float btop = o->CenterZ - o->SizeZ + oz;
    float bbottom = o->CenterZ + o->SizeZ + oz;

    return (left < bright && right > bleft && top < bbottom && bottom > btop);
}

void BB::SetAnimScale(float s)
{
    AnimTargetScale = s;
    Animate = true;
}
