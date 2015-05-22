//============================
//Nagygyörgy Bence 11.b - 2015
//============================
#include "Powerup.h"

#include <Pong3D.h>
#include <algorithm>
#include <Paddle.h>

#include <glm/vec3.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <SoundManager.h>

#include <iostream>
using namespace std;

float Powerup::Radius = 0.3f;

Powerup::Powerup(float x, float y, float z, PowerupTarget trg, PowerupType wot)
{
    X = x;
    Y = y;
    Z = z;

    Target = trg;
    Type = wot;

    Bounds = new BB(-Radius,0,-Radius,Radius,0.2f,Radius,-1);

    Scale = 0.0f;
    Alpha = 0.0f;

    Disappear = false;
}

void Powerup::Update(Game* g)
{
    if(Disappear)
    {
        if(Scale < 1.5f)
        {
            Scale += Pong3D::ElapsedTime * 4;
            Alpha = 1.0f - (Scale - 1.0f) * 2.0f;
        }
        else
        {
            g->Powerups.erase(std::remove(g->Powerups.begin(), g->Powerups.end(), this), g->Powerups.end());
        }
    }
    else
    {
        if(Scale < 1.0f)
        {
            Scale += Pong3D::ElapsedTime * 4;
            Alpha = Scale;
            if(Scale >= 1.0f)
            {
                Scale = 1.0f;
            }
        }
    }
}

void Powerup::Activate(Game* game, Ball* src)
{
    switch(Type)
    {
    case PowerupType::BiggerPad:
    case PowerupType::SmallerPad:
        for(int x = 0;x<game->PlayerCount;x++)
        {
            if(Target == PowerupTarget::All || (Target == PowerupTarget::Self && x == src->LastTouches[0]) || (Target == PowerupTarget::Others && x != src->LastTouches[0]))
            {
                switch(Type)
                {
                    case PowerupType::BiggerPad:
                        game->Pads[x]->TargetSize = Game::BasePadSize * 1.5f;
                        game->Pads[x]->AnimateSize = true;
                        break;
                    case PowerupType::SmallerPad:
                        game->Pads[x]->TargetSize = Game::BasePadSize * 0.5f;
                        game->Pads[x]->AnimateSize = true;
                        break;
                }
            }
        }
        break;
    case PowerupType::DoubleBall:
        for(int x = 0;x<game->Ballz.size();x++)
        {
            if(!game->Ballz[x]->NewBall && game->Ballz.size() < 10)
            {
                glm::vec3 v3 = glm::vec3(game->Ballz[x]->DX, 0.0f, game->Ballz[x]->DZ);
                v3 = glm::rotateY(v3, -15.0f);

                game->Ballz[x]->DX = v3.x;
                game->Ballz[x]->DZ = v3.z;

                v3 = glm::rotateY(v3, 30.0f);

                Ball* b = new Ball(Game::BaseBallSize, game->Ballz[x]->Y);
                b->DX = v3.x;
                b->DZ = v3.z;
                b->X = game->Ballz[x]->X;
                b->Z = game->Ballz[x]->Z;
                b->NewBall = true;
                b->Speed = game->Ballz[x]->Speed;
                b->LastTouches[0] = game->Ballz[x]->LastTouches[0];
                b->LastTouches[1] = game->Ballz[x]->LastTouches[1];

                game->Ballz.push_back(b);
            }
        }
        break;
    case PowerupType::SlowerBall:
    case PowerupType::FasterBall:
        for(int x = 0;x<game->Ballz.size();x++)
        {
            switch(Type)
            {
                case PowerupType::SlowerBall:
                    game->Ballz[x]->Speed = Game::BaseBallSpeed;
                    break;
                case PowerupType::FasterBall:
                    game->Ballz[x]->Speed = Game::BaseBallSpeed * 2.0f;
                    break;
            }
        }
        break;
    case PowerupType::SwapKeys:
        game->SwapButtons = !game->SwapButtons;
        break;
    }
}

Powerup::~Powerup()
{
    delete Bounds;
    Bounds = NULL;
}

bool Powerup::CanSpawn(Game* game, PowerupType type, PowerupTarget &trg)
{
    switch(type){
    case PowerupType::BiggerPad:
        for(Paddle* &p : game->Pads){
            if(p->Size < Game::BasePadSize * 1.5f){
                return true;
            }
        }
        break;
    case PowerupType::SmallerPad:
        for(Paddle* &p : game->Pads){
            if(p->Size > Game::BasePadSize * 0.5f){
                return true;
            }
        }
        break;
    case PowerupType::SlowerBall:
        for(Ball* &b : game->Ballz){
            if(b->Speed > Game::BaseBallSpeed){
                trg = PowerupTarget::All;
                return true;
            }
        }
        break;
    case PowerupType::FasterBall:
        for(Ball* &b : game->Ballz){
            if(b->Speed < Game::BaseBallSpeed * 1.75f){
                trg = PowerupTarget::All;
                return true;
            }
        }
        break;
    case PowerupType::DoubleBall:
        trg = PowerupTarget::All;
        return true;
        break;
    case PowerupType::SwapKeys:
        return true;
        break;
    }

    return false;
}
