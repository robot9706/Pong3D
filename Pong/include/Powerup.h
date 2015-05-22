#ifndef POWERUP_H
#define POWERUP_H

#include <BB.h>
#include <Game.h>
#include <Ball.h>

enum PowerupTarget
{
    All = 0,
    Self = 1,
    Others = 2,

    TrgMax = 3
};

enum PowerupType
{
    BiggerPad = 0,
    SmallerPad = 1,
    FasterBall = 2,
    SlowerBall = 3,
    DoubleBall = 4,
    SwapKeys = 5,

    TypeMax = 6,
};

class Ball;

class Powerup
{
    public:
        Powerup(float x, float y, float z, PowerupTarget trg, PowerupType wot);
        virtual ~Powerup();
        void Update(Game* game);
        void Activate(Game* game, Ball* ball);
        static bool CanSpawn(Game* game, PowerupType type, PowerupTarget &trg);

        BB* Bounds;
        float X;
        float Y;
        float Z;

        bool Disappear;
        float Scale;
        float Alpha;

        PowerupTarget Target;
        PowerupType Type;

        static float Radius;
};

#endif // POWERUP_H
