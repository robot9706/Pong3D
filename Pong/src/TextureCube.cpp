#include "TextureCube.h"

#define GLEW_STATIC

#include <glew.h>
#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
using namespace std;

TextureCube::TextureCube()
{
    glGenTextures(1, &_id);
}

TextureCube::~TextureCube()
{
    glDeleteTextures(1, &_id);
}

bool TextureCube::LoadMem(DataBlock top, DataBlock bottom, DataBlock left, DataBlock right, DataBlock front, DataBlock back)
{
    SDL_RWops *topRW = SDL_RWFromMem(top.Data, top.Size);
    SDL_RWops *bottomRW = SDL_RWFromMem(bottom.Data, bottom.Size);
    SDL_RWops *leftRW = SDL_RWFromMem(left.Data, left.Size);
    SDL_RWops *rightRW = SDL_RWFromMem(right.Data, right.Size);
    SDL_RWops *frontRW = SDL_RWFromMem(front.Data, front.Size);
    SDL_RWops *backRW = SDL_RWFromMem(back.Data, back.Size);

    SDL_Surface *topTex = IMG_LoadPNG_RW(topRW);
    SDL_Surface *bottomTex = IMG_LoadPNG_RW(bottomRW);
    SDL_Surface *leftTex = IMG_LoadPNG_RW(leftRW);
    SDL_Surface *rightTex = IMG_LoadPNG_RW(rightRW);
    SDL_Surface *frontTex = IMG_LoadPNG_RW(frontRW);
    SDL_Surface *backTex = IMG_LoadPNG_RW(backRW);

    if(topTex == NULL || bottomTex == NULL || leftTex == NULL || rightTex == NULL || frontTex == NULL || backTex == NULL)
    {
        cout << "Hiba a kép betöltése közben!" << endl;
        return false;
    }
    else
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, _id);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, topTex->w, topTex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, topTex->pixels);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, bottomTex->w, bottomTex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bottomTex->pixels);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, leftTex->w, leftTex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, leftTex->pixels);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, rightTex->w, rightTex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rightTex->pixels);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, frontTex->w, frontTex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, frontTex->pixels);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, backTex->w, backTex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, backTex->pixels);

        glTexParameteri(GL_TEXTURE_CUBE_MAP , GL_TEXTURE_MIN_FILTER , GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        SDL_FreeSurface(topTex);
        topTex = NULL;

        SDL_FreeSurface(bottomTex);
        bottomTex = NULL;

        SDL_FreeSurface(leftTex);
        leftTex = NULL;

        SDL_FreeSurface(rightTex);
        rightTex = NULL;

        SDL_FreeSurface(frontTex);
        frontTex = NULL;

        SDL_FreeSurface(backTex);
        backTex = NULL;
    }

    SDL_RWclose(topRW);
    SDL_RWclose(bottomRW);
    SDL_RWclose(leftRW);
    SDL_RWclose(rightRW);
    SDL_RWclose(frontRW);
    SDL_RWclose(backRW);

    return true;
}

unsigned int TextureCube::GetID()
{
    return _id;
}
