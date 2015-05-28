#ifndef BB_H
#define BB_H

#include <glm/mat4x4.hpp>

//Egy 3D-s dobozt le�r� oszt�ly
class BB
{
    public:
        BB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, int tag);
        BB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
        BB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, int tag, float startScale);

        float MinX, MinY, MinZ, MaxX, MaxY, MaxZ; //Als� �s fels� sarok
        float SizeX, SizeY, SizeZ; //M�retek
        float CenterX, CenterY, CenterZ; //K�z�ppont

        bool Insecrets(float x, float z, BB* o, float ox, float oz); //Fgv amely megmondja, hogy k�t doboz �tk�zik-e

        glm::mat4 GetWorldMatrix(); //Megadja a doboz "world" m�trix-�t
        glm::mat4 GetWorldMatrixPlane(); //Megadja a doboz "world" m�trix-�t s�khoz

        int Tag; //Egyedi azonos�t�

        //M�ret anim�ci�
        float AnimScale;
        float AnimTargetScale;
        bool Animate;

        void SetAnimScale(float to);
};

#endif // BB_H
