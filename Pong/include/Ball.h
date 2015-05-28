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

        void Update(Game* game); //Frissítõ fgv
        void Render(Game* game); //Rajzoló fgv

        void RandomDir(); //Random irányba indítja a labdát
        void SpawnBall(int player, float ms); //Játékostól függõen helyezi el és indítja el a labdát

        int* LastTouch; //2 elemû tömb amely tartalmazza az elõ két érintõ játékos azonosítóát

        BB* Bounds; //Méretek
        //Pozíció
        float X;
        float Y;
        float Z;

        //Irány vektor
        float DX;
        float DZ;

        float Speed; //Sebesség (rly?)
        bool NewBall; //Jelzi, hogy egy labda új-e, ez a Powerup-hoz kell
};

#endif // BALL_H
