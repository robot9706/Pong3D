#ifndef PADDLE_H
#define PADDLE_H

#include <BB.h>
#include <Game.h>

class Game;

class Paddle
{
    public:
        Paddle(int id, bool dirX, float s, float l, float h, int leftKey, int rightKey);
        virtual ~Paddle();
        float X,Y,Z;
        bool DirectionX;
        float Size;
        float TargetSize;
        bool AnimateSize;
        float Speed;
        float _move;
        float MMax;
        float MMin;
        BB* Bounds;
        int LeftKey;
        int RightKey;
        bool Bot;
        int PlayerID;

        float AnimScale;
        float AnimTargetScale;
        bool Animate;

        bool GotoCenter;

        float BotRandom;

        void Render(Game* game);
        void Update(Game* game, bool input);
        void Move(float v);
};

#endif // PADDLE_H
