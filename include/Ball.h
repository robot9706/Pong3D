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

        int* LastTouches;

        BB* Bounds;
        float X;
        float Y;
        float Z;

        float DX;
        float DZ;

        float Speed;
        bool NewBall;

        void Update(Game* game);
        void Render(Game* game);

        void RandomDir();
        void SpawnBall(int player, float ms);
};

#endif // BALL_H
