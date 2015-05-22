#ifndef TEXTURECUBE_H
#define TEXTURECUBE_H

#include <DataBlock.h>

class TextureCube
{
    public:
        TextureCube();
        bool LoadMem(DataBlock top, DataBlock bottom, DataBlock left, DataBlock right, DataBlock front, DataBlock back);
        virtual ~TextureCube();
        unsigned int GetID();
    private:
        unsigned int _id;
};

#endif // TEXTURECUBE_H
