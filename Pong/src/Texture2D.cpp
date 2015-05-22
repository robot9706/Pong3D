//============================
//Nagygyörgy Bence 11.b - 2015
//============================
#include "Texture2D.h"

#include <glew.h>
#include <SDL.h>
#include <SDL_image.h>

#include <DataBlock.h>

#include <iostream>
#include <string>
using namespace std;

Texture2D::Texture2D()
{
    glGenTextures(1, &_texture);
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &_texture);
}

bool Texture2D::Load(string file)
{
    SDL_Surface* loadedImage = NULL;
    loadedImage = IMG_Load(file.c_str());
    if(loadedImage == NULL)
    {
        cout << "A fájl nem létezik: \"" << file << "\"" << endl;
        return false;
    }
    else
    {
        Width = loadedImage->w;
        Height = loadedImage->h;

        glBindTexture(GL_TEXTURE_2D , _texture);
        glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGBA , loadedImage->w ,
            loadedImage->h , 0 , GL_RGBA , GL_UNSIGNED_BYTE ,
            loadedImage->pixels);

        glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        SDL_FreeSurface(loadedImage);
        loadedImage = NULL;
    }

    return true;
}

bool Texture2D::LoadMem(DataBlock data)
{
    SDL_RWops *rw = SDL_RWFromMem(data.Data, data.Size);
    SDL_Surface *loadedImage = IMG_LoadPNG_RW(rw);

    if(loadedImage == NULL)
    {
        cout << "Hiba a kép betöltése közben!" << endl;
        return false;
    }
    else
    {
        Width = loadedImage->w;
        Height = loadedImage->h;

        glBindTexture(GL_TEXTURE_2D , _texture);
        glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGBA , loadedImage->w ,
            loadedImage->h , 0 , GL_RGBA , GL_UNSIGNED_BYTE ,
            loadedImage->pixels);

        glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        SDL_FreeSurface(loadedImage);
        loadedImage = NULL;
    }

    SDL_RWclose(rw);

    return true;
}

unsigned int Texture2D::GetID()
{
    return _texture;
}
