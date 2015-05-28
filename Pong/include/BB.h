#ifndef BB_H
#define BB_H

#include <glm/mat4x4.hpp>

//Egy 3D-s dobozt leíró osztály
class BB
{
    public:
        BB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, int tag);
        BB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
        BB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, int tag, float startScale);

        float MinX, MinY, MinZ, MaxX, MaxY, MaxZ; //Alsó és felsõ sarok
        float SizeX, SizeY, SizeZ; //Méretek
        float CenterX, CenterY, CenterZ; //Középpont

        bool Insecrets(float x, float z, BB* o, float ox, float oz); //Fgv amely megmondja, hogy két doboz ütközik-e

        glm::mat4 GetWorldMatrix(); //Megadja a doboz "world" mátrix-át
        glm::mat4 GetWorldMatrixPlane(); //Megadja a doboz "world" mátrix-át síkhoz

        int Tag; //Egyedi azonosító

        //Méret animáció
        float AnimScale;
        float AnimTargetScale;
        bool Animate;

        void SetAnimScale(float to);
};

#endif // BB_H
