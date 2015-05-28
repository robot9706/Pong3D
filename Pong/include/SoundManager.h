#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <SDL_mixer.h>
#include <DataBlock.h>

class SoundManager
{
    public:
        static void Init(DataBlock* a); //El�k�sz�ti a hangokhoz sz�ks�ges dolgokat ("a" t�mb tartalmazza a hangot adatait)
        static void Cleanup();

        static void PlayHitSound();
        static void PlayScoreSound();
        static void PlayPowerupSound();

    private:
        static bool SoundOK;

        static Mix_Chunk* _hitSound;
        static Mix_Chunk* _scoreSound;
        static Mix_Chunk* _powerupSound;

        static Mix_Chunk* LoadSound(DataBlock data);
};

#endif // SOUNDMANAGER_H
