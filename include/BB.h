#ifndef BB_H
#define BB_H

#include <glm/mat4x4.hpp>

enum Hit
{
    XN,
    XP,
    ZN,
    ZP,
    None
};

class BB
{
    public:
        BB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, int tag);
        BB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
        BB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, int tag, float startScale);

        float MinX, MinY, MinZ, MaxX, MaxY, MaxZ;
        float SizeX, SizeY, SizeZ;
        float CenterX, CenterY, CenterZ;

        bool Insecrets(float x, float z, BB* o, float ox, float oz);

        glm::mat4 GetWorldMatrix();
        glm::mat4 GetWorldMatrixPlane();

        static Hit HitTest(BB* a, float ax, float az, float aDx, float aDz, BB* b, float bx, float bz, float &in);

        int Tag;

        float AnimScale;
        float AnimTargetScale;
        bool Animate;

        void SetAnimScale(float to);
};

#endif // BB_H
