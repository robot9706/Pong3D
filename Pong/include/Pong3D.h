#ifndef PONG3D_H
#define PONG3D_H

#include <GFX.h>
#include <Game.h>

class Pong3D
{
    public:
        static float TotalRunTime;
        static float ElapsedTime;
        static bool GlobalError;

        Pong3D();
        virtual ~Pong3D();

        bool Init(int ww, int wh, bool fs); //El�k�z�ti a j�t�kot
        void Run(); //Futtatja a j�t�kot :O
        void Exit();

        void HandleEvents();

        void SetTargetFPS(int target);

        GFX* GetGFX();

    private:
        GFX* _gfx;
        SDL_Event* _event;
        Game* _game;

        bool _windowFocus = false;

        int _mx, _my; //Eg�r poz�tic�

        //Id�z�t�s
        bool _running = true;
        int _frameStartTicks;
        int _runStart;
        int _targetFps = 60;
        bool _capFPS = true;
};

#endif // PONG3D_H
