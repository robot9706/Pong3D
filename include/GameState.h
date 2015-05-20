#ifndef GAMESTATE_H
#define GAMESTATE_H

class Pong3D;

class GameState
{
    public:
        GameState(Pong3D* game);
        Pong3D* GameInstance;
        virtual void UpdateState(){}
        virtual void RenderState(){}
};

#endif // GAMESTATE_H
