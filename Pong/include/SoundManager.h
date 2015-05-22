#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <SDL_mixer.h>
#include <DataBlock.h>

class SoundManager
{
    public:
        static void Init(DataBlock* a);
        static void Cleanup();

        static void PlayHitSound();
        static void PlayScoreSound();
        static void PlayPowerupSound();

    private:
        static Mix_Chunk* _hitSound;
        static Mix_Chunk* _scoreSound;
        static Mix_Chunk* _powerupSound;

        static Mix_Chunk* LoadSound(DataBlock data);
};

#endif // SOUNDMANAGER_H
