#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL.h>

class Keyboard
{
    public:
        static void OnKeydown(int code);
        static void OnKeyup(int code);

        static bool IsKeyDown(int id);
        static bool IsKeyUp(int id);

        static int LastKey;

    private:
        static bool _buttonStates[];
};

#endif // KEYBOARD_H
