#ifndef LOADINGSTATE_H
#define LOADINGSTATE_H

#include <GameState.h>
#include <Pong3D.h>
#include <Texture2D.h>

class LoadingState : public GameState
{
    public:
        LoadingState(Pong3D* pong);
        virtual ~LoadingState();
        virtual void UpdateState();
        virtual void RenderState();

        void LoadThread();
    private:
        Texture2D* _texture;
        bool _loading = false;
};

#endif // LOADINGSTATE_H
