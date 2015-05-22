//============================
//Nagygyörgy Bence 11.b - 2015
//============================
#include "Mouse.h"

int Mouse::X = 0;
int Mouse::Y = 0;
bool Mouse::LeftButton = false;
bool Mouse::LeftPress = false;

bool Mouse::HitTest(float rectX, float rectY, float rectW, float rectH)
{
    return (X >= rectX && X <= rectX + rectW && Y >= rectY && Y <= rectY + rectH);
}
