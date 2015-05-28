#include "SoundManager.h"

#include <SDL.h>
#include <SDL_mixer.h>
#include <Pong3D.h>

#include <iostream>
using namespace std;

Mix_Chunk* SoundManager::_hitSound = NULL;
Mix_Chunk* SoundManager::_scoreSound = NULL;
Mix_Chunk* SoundManager::_powerupSound = NULL;

bool SoundManager::SoundOK = true;

void SoundManager::Init(DataBlock* s)
{
    if(!SoundOK)
    {
        cout << "Hang kikapcsolva!" << endl;
        return;
    }

    if(Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024) != 0)
    {
        cout << "Hang hiba!" << endl;
        //Pong3D::GlobalError = true;
        SoundOK = false;
    }
    else
    {
        _hitSound = LoadSound(s[0]);
        _scoreSound = LoadSound(s[1]);
        _powerupSound = LoadSound(s[2]);

        cout << "Hang OK" << endl;
    }
}

void SoundManager::Cleanup()
{
    Mix_FreeChunk(_hitSound);
    Mix_FreeChunk(_scoreSound);
    Mix_FreeChunk(_powerupSound);

    Mix_CloseAudio();
    Mix_Quit();
}

Mix_Chunk* SoundManager::LoadSound(DataBlock data)
{
    SDL_RWops *rw = SDL_RWFromMem(data.Data, data.Size);
    Mix_Chunk* snd = Mix_LoadWAV_RW(rw, 1);
    SDL_RWclose(rw);

    return snd;
}

void SoundManager::PlayHitSound()
{
    if(SoundOK)
        Mix_PlayChannel(-1, _hitSound, 0);
}

void SoundManager::PlayScoreSound()
{
    if(SoundOK)
        Mix_PlayChannel(-1, _scoreSound, 0);
}

void SoundManager::PlayPowerupSound()
{
    if(SoundOK)
        Mix_PlayChannel(-1, _powerupSound, 0);
}

void SoundManager::Disable()
{
    SoundOK = false;
}
