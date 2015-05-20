//============================
//Nagygyörgy Bence 11.b - 2015
//============================
#include "Paddle.h"

#include <Keyboard.h>
#include <Pong3D.h>
#include <cmath>
#include <Ball.h>

#include <iostream>
using namespace std;

Paddle::Paddle(int id, bool dirX, float s, float l, float h, int lk, int rk)
{
    Animate = false;
    AnimScale = 1.0f;
    AnimTargetScale = 1.0f;

    PlayerID = id;
    DirectionX = dirX;

    LeftKey = lk;
    RightKey = rk;

    GotoCenter = false;

    Bot = false;
    Speed = 3.5f;

    AnimateSize = false;

    MMin = 0;
    MMax = 0;

    float xs = 0;
    float zs = 0;

    Size = s;

    BotRandom = (((float)(rand())/(float)(RAND_MAX) - 0.5f) * 2) * 0.2f + 0.2f;

    if(dirX)
    {
        xs = s;
        zs = l;
    }
    else
    {
        zs = s;
        xs = l;
    }

    Bounds = new BB(-xs,0,-zs, xs,h,zs);

    X = 0;
    Y = 0;
    Z = 0;
}

Paddle::~Paddle()
{
    delete Bounds;
    Bounds = NULL;
}

void Paddle::Render(Game* game)
{
    if(DirectionX)
        game->DrawCube(Bounds, X, Y, Z, AnimScale, AnimScale);
    else
        game->DrawCube(Bounds, X, Y, Z, AnimScale, AnimScale);
}

void Paddle::Update(Game* game, bool input)
{
    if(AnimateSize)
    {
        if(Size < TargetSize)
        {
            Size += Pong3D::ElapsedTime;
            if(Size > TargetSize)
            {
                Size = TargetSize;
                AnimateSize = false;
            }
        }
        else
        {
            Size -= Pong3D::ElapsedTime;
            if(Size < TargetSize)
            {
                Size = TargetSize;
                AnimateSize = false;
            }
        }

        if(DirectionX)
        {
            Bounds->SizeX = Size;
        }
        else
        {
            Bounds->SizeZ = Size;
        }
    }

    if(Animate)
    {
        if(AnimScale < AnimTargetScale)
        {
            AnimScale += Pong3D::ElapsedTime * 5;
            if(AnimScale >= 1.0f)
            {
                AnimScale = 1.0f;
                Animate = false;
            }
        }
        else if(AnimScale > AnimTargetScale)
        {
            AnimScale -= Pong3D::ElapsedTime * 5;
            if(AnimScale <= 0.0f)
            {
                AnimScale = 0.0f;
                Animate = false;
            }
        }
        else
        {
            Animate = false;
        }
    }

    if(GotoCenter)
    {
        float p = 0.0f;
        if(DirectionX)
            p = X;
        else
            p = Z;

        float ch = abs(p);
        if(ch > 0.5f)
        {
            if(p<0)
                Move(1.0f);
            else
                Move(-1.0f);
        }
        else if(ch > 0.2f)
        {
            _move *= 0.95f;
        }
        else
        {
            _move = 0.0f;
            GotoCenter = false;
        }
    }

    if(Bot && !GotoCenter && input)
    {
        if(game->Ballz.size() > 0)
        {
            //Megkeresem a lekgözelebbi labdát
            Ball* b = NULL;
            float bd;
            for(Ball* bi : game->Ballz)
            {
                if(b == NULL)
                {
                    b = bi;
                    bd = sqrt(pow(bi->X - X, 2) + pow(bi->Z - Z, 2));
                }
                else
                {
                    float d = sqrt(pow(bi->X - X, 2) + pow(bi->Z - Z, 2));
                    if(d<bd)
                    {
                        bd = d;
                        b = bi;
                    }
                }
            }

            float ch = 0.0f;
            if(DirectionX)
                ch = b->X - X;
            else
                ch = b->Z - Z;
            if(abs(ch) > 0.25f)
            {
                if(ch < 0)
                    Move(-1.0f);
                else
                    Move(1.0f);
            }
            else
                 Move(-BotRandom);
        }
        else
            _move *= 0.95f;
    }
    else if(input)
    {
        if(Keyboard::IsKeyDown(LeftKey))
            Move(-1.0f);
        else if(Keyboard::IsKeyDown(RightKey))
            Move(1.0f);
        else
            _move *= 0.95f;
    }

    if(DirectionX)
    {
        X += _move * Speed * Pong3D::ElapsedTime;
        if(X - Size < MMin)
        {
            X = MMin + Size;
            _move = 0;
        }
        else if(X + Size > MMax)
        {
            X = MMax - Size;
            _move = 0;
        }
    }
    else
    {
        Z += _move * Speed * Pong3D::ElapsedTime;
        if(Z - Size < MMin)
        {
            Z = MMin + Size;
            _move = 0;
        }
        else if(Z + Size > MMax)
        {
            Z = MMax - Size;
            _move = 0;
        }
    }
}

void Paddle::Move(float v)
{
    _move += 5 * v * Pong3D::ElapsedTime;
    if(_move < -1)
        _move = -1;
    else if(_move > 1)
        _move = 1;
}
