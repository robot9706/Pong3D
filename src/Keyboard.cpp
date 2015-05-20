//============================
//Nagygyörgy Bence 11.b - 2015
//============================
#include "Keyboard.h"

#include <SDL.h>

bool Keyboard::_buttonStates[255];
int Keyboard::LastKey = 0;

void Keyboard::OnKeydown(int code)
{
    _buttonStates[code] = true;
    LastKey = code;
}

void Keyboard::OnKeyup(int code)
{
    _buttonStates[code] = false;
}

bool Keyboard::IsKeyDown(int id)
{
    return _buttonStates[id];
}

bool Keyboard::IsKeyUp(int id)
{
    return _buttonStates[id];
}
