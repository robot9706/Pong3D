#ifndef BALL_H
#define BALL_H

#include <BB.h>
#include <Game.h>

class Game;

class Ball
{
    public:
        Ball(float s, float y);
        virtual ~Ball();

        void Update(Game* game); //Friss�t� fgv
        void Render(Game* game); //Rajzol� fgv

        void RandomDir(); //Random ir�nyba ind�tja a labd�t
        void SpawnBall(int player, float ms); //J�t�kost�l f�gg�en helyezi el �s ind�tja el a labd�t

        int* LastTouch; //2 elem� t�mb amely tartalmazza az el� k�t �rint� j�t�kos azonos�t��t

        BB* Bounds; //M�retek
        //Poz�ci�
        float X;
        float Y;
        float Z;

        //Ir�ny vektor
        float DX;
        float DZ;

        float Speed; //Sebess�g (rly?)
        bool NewBall; //Jelzi, hogy egy labda �j-e, ez a Powerup-hoz kell
};

#endif // BALL_H
