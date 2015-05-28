#ifndef MOUSE_H
#define MOUSE_H


class Mouse
{
    public:
        static int X;
        static int Y;
        static bool LeftButton;
        static bool LeftPress;
        static bool HitTest(float rectX, float rectY, float rectW, float rectH); //Megmondja, hogy az egér benne van-e a téglalapban
};

#endif // MOUSE_H
