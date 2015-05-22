//============================
//Nagygyörgy Bence 11.b - 2015
//============================
#include "Ball.h"

#include <Game.h>
#include <Pong3D.h>
#include <cmath>
#include <algorithm>
#include <Powerup.h>
#include <SoundManager.h>

Ball::Ball(float s, float y)
{
    X = 0;
    Z = 0;
    Y = y;
    float hs = s / 2.0f;
    Bounds = new BB(-hs,-hs,-hs,hs,hs,hs);

    LastTouches = new int[2]{-1,-1};
    Speed = Game::BaseBallSpeed;;

    DX = 0.0f;
    DZ = 0.0f;

    NewBall = true;
}

Ball::~Ball()
{
    delete Bounds;
    Bounds = NULL;

    delete [] LastTouches;
    LastTouches = NULL;
}

void Ball::RandomDir()
{
    DX = 0.0f;
    DZ = 0.0f;
    switch(rand() % 4)
    {
    case 0:
        DX = 1.0f;
        break;
    case 1:
        DX = -1.0f;
        break;
    case 2:
        DZ = 1.0f;
        break;
    case 3:
        DZ = -1.0f;
        break;
    }
}

void Ball::Update(Game* game)
{
    X += DX * Speed * Pong3D::ElapsedTime;
    Z += DZ * Speed * Pong3D::ElapsedTime;

    for(Paddle* &p : game->Pads)
    {
        if(p->AnimScale > 0.0f && Bounds->Insecrets(X, Z, p->Bounds, p->X, p->Z))
        {
            SoundManager::PlayHitSound();

            if(LastTouches[0] != p->PlayerID)
            {
                if(LastTouches[1] != LastTouches[0])
                    LastTouches[1] = LastTouches[0];

                LastTouches[0] = p->PlayerID;
            }

            if(p->DirectionX)
            {
                DZ = DZ * -1;
                Z += DZ * 2 * Speed * Pong3D::ElapsedTime;

                float dx = (p->X - X) / p->Size;
                DX = DX * 0.5f + dx * -0.5f;
                if(dx < -0.95f)
                    DX = 1;
                else if(dx > 0.95f)
                    DX = -1.0f;
            }
            else
            {
                DX = DX * -1;
                X += DX * 2 * Speed * Pong3D::ElapsedTime;

                float dz = (p->Z - Z) / p->Size;
                DZ = DZ * 0.5f + dz * -0.5f;
                if(dz < -0.95f)
                    DZ = 1;
                else if(dz > 0.95f)
                    DZ = -1.0f;
            }

            float d = sqrt(pow(DX, 2) + pow(DZ, 2));
            DX = DX / d;
            DZ = DZ / d;
        }
    }

    for(BB* &b : game->Map)
    {
        if(b->AnimScale > 0.0f && Bounds->Insecrets(X, Z, b, 0, 0))
        {
            if(b->Tag == -1)
            {
                SoundManager::PlayHitSound();

                //Melyik oldal?
                float w = 0.5 * (Bounds->SizeX + b->SizeX);
                float h = 0.5 * (Bounds->SizeZ + b->SizeZ);
                float dx = (Bounds->CenterX + X) - b->CenterX;
                float dz = (Bounds->CenterZ + Z) - b->CenterZ;

                float wz = w * dz;
                float hx = h * dx;

                if (wz > hx)
                    if (wz > -hx)
                        DZ = DZ * -1; //felsõ
                    else
                        DX = DX * -1; //bal
                else
                    if (wz > -hx)
                        DX = DX * -1; //jobb
                    else
                        DZ = DZ * -1; //alsó

                float d = sqrt(pow(DX, 2) + pow(DZ, 2));
                DX = DX / d;
                DZ = DZ / d;
            }
            else
            {
                SoundManager::PlayScoreSound();

                game->Ballz.erase(std::remove(game->Ballz.begin(), game->Ballz.end(), this), game->Ballz.end());

                int touch = -1;
                if(LastTouches[0] == b->Tag)
                    touch = LastTouches[1];
                else
                    touch = LastTouches[0];

                game->OnBallRemoved(touch, b->Tag);
                return;
            }
        }
    }

    for(Powerup* &p : game->Powerups)
    {
        if(!p->Disappear)
        {
            float dsta = abs(p->X - X);
            float dstb = abs(p->Z - Z);

            float dst = sqrt(pow(dsta - Bounds->SizeX, 2) + pow(dstb - Bounds->SizeZ, 2));
            if(dsta<(Bounds->SizeX + Powerup::Radius) && dstb < (Bounds->SizeZ + Powerup::Radius))
            {
                if(dsta <= Bounds->SizeX || dstb <= Bounds->SizeZ || dst < pow(Powerup::Radius, 2))
                {
                    p->Disappear = true;
                    p->Activate(game, this);

                    SoundManager::PlayPowerupSound();
                }
            }
        }
    }

    if(X<-Game::MapSize-2 || X > Game::MapSize+2 || Z<-Game::MapSize-2 || Z>Game::MapSize+2)
    {
        game->Ballz.erase(std::remove(game->Ballz.begin(), game->Ballz.end(), this), game->Ballz.end());
        game->OnBallRemoved(-1, -1);
    }
}

void Ball::Render(Game* game)
{
    game->DrawSphere(Bounds, X, Y, Z, 1.0f);
}

void Ball::SpawnBall(int p, float ms)
{
    switch(p){
    case 0:
        X = 0.0f;
        Z = -ms / 3;
        DX = 0.0f;
        DZ = -1.0f;
        break;
    case 1:
        X = 0.0f;
        Z = ms / 3;
        DX = 0.0f;
        DZ = 1.0f;
        break;
    case 2:
        X = -ms / 3;
        Z = 0.0f;
        DX = -1.0f;
        DZ = 0.0f;
        break;
    case 3:
        X = ms / 3;
        Z = 0.0f;
        DX = 1.0f;
        DZ = 0.0f;
        break;
    }
}
