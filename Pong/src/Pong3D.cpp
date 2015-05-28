//============================
//Nagygyörgy Bence 11.b - 2015
//============================
#include "Pong3D.h"

#define GLEW_STATIC

#include <SDL.h>
#include <glew.h>
#include <Keyboard.h>
#include <iostream>
#include <SpriteBatch.h>
#include <GameState.h>
#include <LoadingState.h>
#include <Mouse.h>

float Pong3D::ElapsedTime = 0;
float Pong3D::TotalRunTime = 0;

bool Pong3D::GlobalError = false;

Pong3D::Pong3D()
{
    _gfx = NULL;

    _mx = 0;
    _my = 0;
}

bool Pong3D::Init(int ww, int wh, bool fs)
{
    _gfx = new GFX("Pong3D", ww, wh, 8);

    if(!_gfx->Init())
        return false;

    if(fs)
        _gfx->SetFullscreen(true);

    _event = new SDL_Event();

    //OpenGL verzió ellenõrzés
    const GLubyte* glVer = glGetString(GL_VERSION);
    int major = (int)glVer[0] - 48;
    int minor = (int)glVer[2] - 48;

    cout << "OpenGL: " << glVer << endl;
    if(major < 2 || !glewIsSupported("GL_VERSION_2_0"))
    {
        cout << "Minimum OpenGL 2.0 szükséges!";
        return false;
    }
    else
    {
        cout << "OpenGL OK" << endl;
    }

    //OpenGL alap beállítások
    glEnable(GL_MULTISAMPLE); //Élsímítás

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST); //Mélység buffer

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //Szebb perspektív nézet

    //Átlátszóság
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f, 1); //Kukoricavirágkék O.o

    //Spritebatch elõkészítése
    SpriteBatch::Instance = new SpriteBatch(this);

    _game = new Game(this);

    return true;
}

void Pong3D::Run()
{
    _runStart = SDL_GetTicks();
    _frameStartTicks = SDL_GetTicks(); //Elkezdem mérni az eltelt idõt
    while(_running) //Amég fut a játék
    {
        HandleEvents(); //Esemény kezelés (ablak események, billentyûzet)

        //Kép
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //"Letakarítom" a képet

            _game->Update();
            if(_windowFocus)
                _game->Render();

            if(Mouse::LeftPress)
                Mouse::LeftPress = false;

            if(GlobalError) //Valamilyen hiba történt ami miatt nem tudok tovább futni :/
                return;
        }

        if(_windowFocus)
            _gfx->SwapBuffer(); //Kicserélem az elsõ és hátsó buffereket így megjelenik a kép ^^

        TotalRunTime = (float)(SDL_GetTicks() - _runStart) / 1000;

        int ticks = SDL_GetTicks() - _frameStartTicks;

        if(_capFPS == true) //Ha van FPS korlát
        {
            int left = (1000 / _targetFps) - ticks;
            if(left > 0)
                SDL_Delay(left / 2); //Várok, hogy kitöltsem a maradék idõt a képbõl (/2 -> wut)

            ticks = SDL_GetTicks() - _frameStartTicks;
            ElapsedTime = 1.0f / (float)_targetFps;
        }
        else
        {
            ElapsedTime = (float)ticks / 1000.0f;
        }

        _frameStartTicks = SDL_GetTicks();
    }

    SDL_Quit();
}

void Pong3D::Exit()
{
    _running = false;
}

void Pong3D::HandleEvents()
{
    while(SDL_PollEvent(_event))
    {
        switch(_event->type)
        {
        case SDL_QUIT:
            _running = false;
            break;
        case SDL_KEYDOWN:
            Keyboard::OnKeydown((int)_event->key.keysym.scancode);
            break;
        case SDL_KEYUP:
            Keyboard::OnKeyup((int)_event->key.keysym.scancode);
            break;
        case SDL_WINDOWEVENT:
            {
                switch(_event->window.event)
                {
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                        _windowFocus = true;
                        break;
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                        _windowFocus = false;
                        break;
                    case SDL_WINDOWEVENT_CLOSE:
                        _running = false;
                        break;
                    case SDL_WINDOWEVENT_RESIZED:
                        _gfx->SizeChanged();
                        break;
                }
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(SDL_BUTTON(SDL_GetMouseState(&_mx, &_my)) == SDL_BUTTON_LEFT){
                Mouse::LeftButton = true;
                Mouse::LeftPress = true;

                Mouse::X = (int)(((float)_mx / (float)_gfx->GetContextWidth()) * 1280.0f);
                Mouse::Y = (int)(((float)_my / (float)_gfx->GetContextHeight()) * 720.0f);
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if(SDL_BUTTON(SDL_GetMouseState(&_mx, &_my)) == SDL_BUTTON_LEFT){
                Mouse::LeftButton = false;
            }
            break;
        }
    }
}

void Pong3D::SetTargetFPS(int target)
{
    _capFPS = (target > 0);
    _targetFps = target;

    cout << "FPS cap: " << (_capFPS ? "true" : "false") << " Target FPS: " << _targetFps << endl;
}

GFX* Pong3D::GetGFX()
{
    return _gfx;
}

Pong3D::~Pong3D()
{
    delete _gfx;
    _gfx = NULL;

    delete _event;
    _event = NULL;
}
