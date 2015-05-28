//============================
//Nagygyörgy Bence 11.b - 2015
//============================
#include "GFX.h"

#include <SDL.h>
#include <SDL_image.h>

#define GLEW_STATIC

#include <glew.h>
#include <windows.h>

#include <iostream>

using namespace std;

GFX::GFX(string title, int width, int height, int bpc)
{
    _windowTitle = title;
    _contextWidth = width;
    _contextHeight = height;
    _bitsPerComponent = bpc;

    _window = NULL;
    _glContext = NULL;
    _displayMode = NULL;

    _fullscreen = false;

    _currentDisplay = 0;
}

bool GFX::Init()
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        MessageBox(NULL, (string("SDL hiba: ") + SDL_GetError()).c_str(), "Hiba", MB_OK | MB_ICONHAND);
        return false;
    }

    //SDL beállítások
    {
        //RGB bitek
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, _bitsPerComponent);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, _bitsPerComponent);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, _bitsPerComponent);

        //"Mélység" buffer bit száma
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

        //2 buffer: elsõ + hátsó
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        //1db élsímító buffer
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);

        //2x-es élsímítás :D
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);

        //GPU használata
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    }

    //Monitor fölbontás
    _displayMode = new SDL_DisplayMode();
    SDL_GetCurrentDisplayMode(_currentDisplay, _displayMode);

    //Készítek egy SDL ablakot
    _window = SDL_CreateWindow(_windowTitle.c_str(), (_displayMode->w / 2) - (_contextWidth / 2), (_displayMode->h / 2) - (_contextHeight / 2),
                               _contextWidth, _contextHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_GetWindowSize(_window, &_contextWidth, &_contextHeight); //Eltárolom az ablak kliens méretét

    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        MessageBox(NULL, (string("SDL hiba: ") + SDL_GetError()).c_str(), "Hiba", MB_OK | MB_ICONHAND);
        return false;
    }

    //OpenGL
    SDL_GLContext gl = SDL_GL_CreateContext(_window);
    _glContext = &gl;

    //GLEW
    if(glewInit() != GLEW_OK)
    {
        MessageBox(NULL, "GLEW hiba :(", "Hiba", MB_OK | MB_ICONHAND);
        return false;
    }

    glViewport(0, 0, _contextWidth, _contextHeight);

    //Minden k
    return true;
}

void GFX::SizeChanged()
{
    SDL_GetWindowSize(_window, &_contextWidth, &_contextHeight); //Eltárolom az ablak kliens méretét

    glViewport(0, 0, _contextWidth, _contextHeight);
}

void GFX::SwapBuffer()
{
    SDL_GL_SwapWindow(_window);
}

int GFX::GetContextWidth()
{
    return _contextWidth;
}

int GFX::GetContextHeight()
{
    return _contextHeight;
}

void GFX::SetFullscreen(bool fs)
{
    _fullscreen = fs;
    SDL_SetWindowFullscreen(_window, (_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0));
}

void GFX::ToggleFullscreen()
{
    SetFullscreen(!_fullscreen);
}

GFX::~GFX()
{
    delete _displayMode;

    SDL_GL_DeleteContext(_glContext);
    SDL_DestroyWindow(_window);
}
