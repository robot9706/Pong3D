#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <string>
using namespace std;

class Texture2D
{
    public:
        Texture2D();
        virtual ~Texture2D();

        bool Load(string file);
        unsigned int GetID();

        int Width;
        int Height;
    private:
        unsigned int _texture;
};

#endif // TEXTURE2D_H
