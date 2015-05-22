#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <DataBlock.h>

#include <string>
using namespace std;

class Texture2D
{
    public:
        Texture2D();
        virtual ~Texture2D();

        bool Load(string file);
        bool LoadMem(DataBlock data);
        unsigned int GetID();

        int Width;
        int Height;
    private:
        unsigned int _texture;
};

#endif // TEXTURE2D_H
