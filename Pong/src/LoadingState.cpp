//============================
//Nagygyörgy Bence 11.b - 2015
//============================
#include "LoadingState.h"
#include <iostream>
#include <SpriteBatch.h>
#include <Texture2D.h>
#include <thread>

#include <thread>

using namespace std;

LoadingState::LoadingState(Pong3D* pong) : GameState(pong)
{
    _texture = new Texture2D();
    if(!_texture->Load("Data/WhitePixel.png"))
        Pong3D::GlobalError = true;
}

LoadingState::~LoadingState()
{
    delete _texture;
    _texture = NULL;
}

void LoadingState::UpdateState()
{
}

void LoadingState::RenderState()
{
    SpriteBatch::Singelton->Begin();

    SpriteBatch::Singelton->SetColor(0,0,0,1);
    SpriteBatch::Singelton->DrawTexture(_texture, 0, 0, GameInstance->GetGFX()->GetContextWidth(), GameInstance->GetGFX()->GetContextHeight(), 0, 0, 0);

    SpriteBatch::Singelton->SetColor(1,1,1,1);
    SpriteBatch::Singelton->DrawTexture(_texture, GameInstance->GetGFX()->GetContextWidth() - 64.0f, GameInstance->GetGFX()->GetContextHeight() - 64.0f,
                                        48.0f, 48.0f, 24.0f, 24.0f, Pong3D::TotalRunTime * 180);
    SpriteBatch::Singelton->End();
}

void LoadingState::LoadThread()
{

}
