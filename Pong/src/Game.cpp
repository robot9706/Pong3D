//============================
//Nagygyörgy Bence 11.b - 2015
//============================
#include "Game.h"

#include <iostream>
#include <Pong3D.h>
#include <cmath>
#include <Mouse.h>
#include <Keyboard.h>
#include <PlaneRenderer.h>
#include <string>
#include <SphereRenderer.h>
#include <CubeRenderer.h>
#include <UVPlane.h>
#include <UVCubeRenderer.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Data.h>
#include <DataBlock.h>
#include <SoundManager.h>

#include <glew.h>

using namespace std;

string Game::DiffuseColorVS=
"#version 110\n"
"attribute vec3 vPos;"
"attribute vec3 vNorm;"
"uniform mat4 PV;"
"uniform mat4 W;"
"uniform vec3 LD;"
"varying float DiffuseValue;"
"void main()"
"{"
"   gl_Position = PV * W * vec4(vPos, 1);"
"   DiffuseValue = min(1.0, max(0.0, dot(normalize(LD), vNorm)) + 0.4);"
"}";

string Game::DiffuseColorFS =
"#version 110\n"
"varying float DiffuseValue;"
"uniform vec3 BaseColor;"
"void main()"
"{"
"   gl_FragColor = vec4(BaseColor * DiffuseValue, 1);"
"}";

string Game::DiffuseTextureVS=
"#version 110\n"
"attribute vec3 vPos;"
"attribute vec3 vNorm;"
"attribute vec2 inUV;"
"uniform mat4 PV;"
"uniform mat4 W;"
"uniform vec3 LD;"
"varying float DiffuseValue;"
"varying vec2 UV;"
"void main()"
"{"
"   UV = inUV;"
"   gl_Position = PV * W * vec4(vPos, 1);"
"   DiffuseValue = min(1.0, max(0.0, dot(normalize(LD), vNorm)) + 0.4);"
"}";

string Game::DiffuseTextureFS =
"#version 110\n"
"varying float DiffuseValue;"
"varying vec2 UV;"
"uniform sampler2D Texture;"
"uniform vec3 BaseColor;"
"void main()"
"{"
"   vec4 tex = texture2D(Texture, UV);"
"   if(tex.a < 0.1) discard;"
"   gl_FragColor = vec4(tex.rgb * BaseColor * DiffuseValue, tex.a);"
"}";

string Game::DiffuseTextureFS2 =
"#version 110\n"
"varying float DiffuseValue;"
"varying vec2 UV;"
"uniform sampler2D Texture;"
"uniform vec4 BaseColor;"
"void main()"
"{"
"   vec4 tex = texture2D(Texture, UV);"
"   float a = BaseColor.a * tex.a;"
"   if(a < 0.05) discard;"
"   gl_FragColor = vec4(tex.rgb * BaseColor.rgb * DiffuseValue, a);"
"}";

string Game::SkyboxVS =
"#version 110\n"
"attribute vec3 vPos;"
"attribute vec3 vNorm;"
"varying vec3 texNorm;"
"uniform mat4 PV;"
"uniform mat4 W;"
"void main()"
"{"
"   texNorm = vNorm;"
"   gl_Position = PV * W * vec4(vPos, 1);"
"}";

string Game::SkyboxFS =
"varying vec3 texNorm;"
"uniform samplerCube TexCube;"
"void main()"
"{"
"   gl_FragColor = textureCube(TexCube, texNorm);"
"}";

float Game::MapSize = 6.0f;
float Game::BasePadSize = 0.75f;
float Game::BaseBallSize = 0.25f;
float Game::BaseBallSpeed = 7.5f;
float Game::BaseCameraRotationSpeed = 10.0f;

Game::Game(Pong3D* p)
{
    _pong = p;

    _state = State::Menu;
    _batch = SpriteBatch::Instance;

    SoundManager::Init(new DataBlock[3]{hit_wav,score_wav,powerup_activate_wav});

    //Pálya
    {
        float ms = MapSize;
        wallSize = 0.5f;
        mapH = 1.0f;
        cornerSize = 2.5f;
        gateSize = 1;

        float lineL = MapSize - wallSize - cornerSize;

        Map.push_back(new BB(-ms, 0.0f, -ms, -ms + wallSize, mapH, ms, 2, 1.0f)); //Felsö fal - 0
        Map.push_back(new BB(ms, 0.0f, ms, ms - wallSize, mapH, -ms, 3, 1.0f)); //Alsó fal - 1

        Map.push_back(new BB(-ms + wallSize, 0, -ms, ms - wallSize, mapH, -ms + wallSize, 0, 1.0f)); //Jobb nagy fal - 2
        Map.push_back(new BB(-ms + wallSize, 0, ms - wallSize, ms - wallSize, mapH, ms, 1, 1.0f)); //Bal nagy fal - 3

        Map.push_back(new BB(-ms + wallSize, 0, -ms + wallSize, -ms + wallSize + cornerSize, mapH, -ms + wallSize + cornerSize, -1, 1.0f)); //Jobb felsö sarok - 4
        Map.push_back(new BB(-ms + wallSize, 0, ms - wallSize, -ms + wallSize + cornerSize, mapH, ms - wallSize - cornerSize, -1, 1.0f)); //Bal felsö sarok - 5
        Map.push_back(new BB(ms - wallSize, 0, -ms + wallSize, ms - wallSize - cornerSize, mapH, -ms + wallSize + cornerSize, -1, 1.0f)); //Jobb alsó sarok - 6
        Map.push_back(new BB(ms - wallSize, 0, ms - wallSize, ms - wallSize - cornerSize, mapH, ms - wallSize - cornerSize, -1, 1.0f)); //Bal alsó sarok - 7

        Map.push_back(new BB(-lineL, 0.0f, -ms, -lineL - wallSize, mapH, ms, -1, 1.0f)); //Felsö kis fal - 8
        Map.push_back(new BB(lineL, 0.0f, ms,lineL + wallSize, mapH, -ms, -1, 1.0f)); //Alsó kis fal - 9

        Map.push_back(new BB(-lineL, 0, -ms, lineL, mapH, -ms + wallSize, 0, 1.0f)); //Jobb kis fal - 10
        Map.push_back(new BB(-lineL, 0, ms - wallSize, lineL, mapH, ms, 1, 1.0f)); //Bal kis fal - 11

        Map.push_back(new BB(-ms + wallSize, 0, -ms, lineL, mapH, -ms + wallSize, 0, 1.0f)); //Jobb fura fal - 12
        Map.push_back(new BB(-ms + wallSize, 0, ms - wallSize, lineL, mapH, ms, 1, 1.0f)); //Bal fura fal - 13
        Map.push_back(new BB(lineL, 0.0f, ms,lineL + wallSize, mapH, -ms, -1, 1.0f)); //Alsó fura fal - 14
    }

    //Ütök
    {
        Paddle* p = new Paddle(0, true, BasePadSize, 0.1f, 1.0f, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT);
        p->Z = -MapSize + wallSize + gateSize - 0.1f;
        p->MMax = MapSize - wallSize - cornerSize;
        p->MMin = -MapSize + wallSize + cornerSize;
        p->Bot = true;
        Pads.push_back(p);

        Paddle* p2 = new Paddle(1, true, BasePadSize, 0.1f, 1.0f, SDL_SCANCODE_A, SDL_SCANCODE_S);
        p2->Z = MapSize - wallSize - gateSize + 0.1f;
        p2->MMax = MapSize - wallSize - cornerSize;
        p2->MMin = -MapSize + wallSize + cornerSize;
        p2->Bot = true;
        Pads.push_back(p2);

        Paddle* p3 = new Paddle(2, false, BasePadSize, 0.1f, 1.0f, SDL_SCANCODE_N, SDL_SCANCODE_B);
        p3->X = -MapSize + wallSize + gateSize - 0.1f;
        p3->MMax = MapSize - wallSize - cornerSize;
        p3->MMin = -MapSize + wallSize + cornerSize;
        p3->Bot = true;
        Pads.push_back(p3);

        Paddle* p4 = new Paddle(3, false, BasePadSize, 0.1f, 1.0f, SDL_SCANCODE_P, SDL_SCANCODE_O);
        p4->X = MapSize - wallSize - gateSize + 0.1f;
        p4->MMax = MapSize - wallSize - cornerSize;
        p4->MMin = -MapSize + wallSize + cornerSize;
        p4->Bot = true;
        Pads.push_back(p4);
    }

    _menuBG = new Texture2D();
    LoadTextureMem(_menuBG, MenuBG_png); //"Data/MenuBG.png"

    _menu = new Texture2D();
    LoadTextureMem(_menu, Menu_png); //"Data/Menu.png"

    _pongLogo = new Texture2D();
    LoadTextureMem(_pongLogo, PongLogo_png);

    _startBTN = new Texture2D();
    LoadTextureMem(_startBTN, StartBTN_png);

    _okBTN = new Texture2D();
    LoadTextureMem(_okBTN, OkBTN_png);

    DataBlock pKeys[] = {P1Keys_png,P2Keys_png,P3Keys_png,P4Keys_png};
    _pButtons = new Texture2D[4];
    for(int x = 0;x<4;x++){
        LoadTextureMem(&_pButtons[x], pKeys[x]);
    }

    DataBlock pHelp[] = {P1Movement_png,P2Movement_png,P3Movement_png, P4Movement_png};
    _pButtonHelp = new Texture2D[4];
    for(int x = 0;x<4;x++){
        LoadTextureMem(&_pButtonHelp[x], pHelp[x]);
    }

    DataBlock pNames[] = {P1_png,P2_png,P3_png, P4_png};
    _pNames = new Texture2D[4];
    for(int x = 0;x<4;x++){
        LoadTextureMem(&_pNames[x], pNames[x]);
    }

    _botBTN = new Texture2D();
    LoadTextureMem(_botBTN, BotBTN_png);

    _winz = new Texture2D();
    LoadTextureMem(_winz, Winz_png);

    _xBTN = new Texture2D();
    LoadTextureMem(_xBTN, XButton_png);

    DataBlock numz[] = {Num0_png, Num1_png, Num2_png, Num3_png, Num4_png, Num5_png, Num6_png, Num7_png, Num8_png, Num9_png};
    _numbers = new Texture2D[10];
    for(int x = 0;x<10;x++){
        LoadTextureMem(&_numbers[x], numz[x]);
    }

    DataBlock pws[] = {BiggerPad_png,SmallerPad_png,FasterBall_png,SlowerBall_png,DoubleBall_png,SwapKeys_png};
    _powerupImages = new Texture2D[(int)PowerupType::TypeMax];
    for(int x = 0;x<(int)PowerupType::TypeMax;x++){
        LoadTextureMem(&_powerupImages[x], pws[x]);
    }

    _skybox = new TextureCube();
    if(!_skybox->LoadMem(Skybox_top_png, Skybox_bottom_png, Skybox_left_png, Skybox_right_png, Skybox_front_png, Skybox_back_png)){
        Pong3D::GlobalError = true;
        cout << "Hiba az égbolt betöltése közben!" << endl;
    }

    SwapButtons = false;

    menuX = (1280 / 2 - _menuBG->Width / 2);
    menuY = (720 / 2 - _menuBG->Height / 2 + 720 / 10);
    menuW = _menuBG->Width;
    menuH = _menuBG->Height;

    btnsX = menuX + (menuW / 2) - ((&_pButtons[0])->Width * 0.85f);
    btnsY = menuY + (menuH / 8) + 15;
    btnPadding = (&_pButtons[0])->Height;

    Texture2D* winner = &_pNames[0];
    float fullW = winner->Width + _winz->Width;
    nameWidth = winner->Width / fullW;
    winzWidth = _winz->Width / fullW;
    nameWidth = nameWidth * menuW;
    winzWidth = winzWidth * menuW;
    textHeight = (nameWidth / winner->Width) * winner->Height;

    Texture2D* num = &_numbers[0];
    lineY = menuY + textHeight + 20;
    lineW = winner->Width + num->Width + 20;
    lineX = menuX + (menuW / 2.0f) - (lineW / 2.0f);
    winPadding = 15;

    winnumH = winner->Height;
    winnumW = (winnumH / (float)num->Height) * (float)num->Width;

    camLookX = 0.0f;
    camRenderX = MapSize;
    camRenderZ = MapSize;
    camRotation = 0.0f;

    _playerPoints = new int[4]{0,0,0,0};
    _targetPoints = 10;
    _finishOrder = new int[4]{0,0,0,0};
    _finishPoints = new int[4]{0,0,0,0};

    _powerupSpawnTime = 0.0f;
    _escTime = 0.0f;

    _countdown = false;
    _initialCountdown = false;

    PlayerCount = 4;
    BuildMap(4); //Alap pálya

    Ballz.push_back(new Ball(BaseBallSize, mapH / 2.0f));
    Ballz[0]->RandomDir();

    PlaneRenderer::Init();
    CubeRenderer::Init();
    UVPlane::Init();
    SphereRenderer::Init();

    glm::vec3 lightDir = glm::vec3(-0.25f,0.5f,0.75f);
    lightDir = glm::normalize(lightDir);

    //Diffúz szín shader
    _diffuseColorShader = new Shader();
    if(!_diffuseColorShader->Load(DiffuseColorVS, DiffuseColorFS)){
        cout<<"Diffuse shader hiba!"<<endl;
        Pong3D::GlobalError = true;
        return;
    }

    _diffuseColorShader->Apply();
    _diffuseColorShader->BindLocation("vPos", 0);
    _diffuseColorShader->BindLocation("vNorm", 1);
    _difBaseColor = _diffuseColorShader->GetUniform("BaseColor");
    _difPV = _diffuseColorShader->GetUniform("PV");
    _difW = _diffuseColorShader->GetUniform("W");
    unsigned int ldUniform = _diffuseColorShader->GetUniform("LD");
    _diffuseColorShader->SetVector3(ldUniform, lightDir.x, lightDir.y, lightDir.z);

    //Diffúz textúra shader
    _diffuseTextureShader = new Shader();
    if(!_diffuseTextureShader->Load(DiffuseTextureVS, DiffuseTextureFS)){
        cout << "Diffuse texture shader hiba!"<<endl;
        Pong3D::GlobalError = true;
        return;
    }

    _diffuseTextureShader->Apply();
    _diffuseTextureShader->BindLocation("vPos", 0);
    _diffuseTextureShader->BindLocation("vNorm", 1);
    _diffuseTextureShader->BindLocation("vUV", 2);
    _difTexBaseColor = _diffuseTextureShader->GetUniform("BaseColor");
    _difTexSampler = _diffuseTextureShader->GetUniform("Texture");
    _difTexPV = _diffuseTextureShader->GetUniform("PV");
    _difTexW = _diffuseTextureShader->GetUniform("W");
    ldUniform = _diffuseTextureShader->GetUniform("LD");
    _diffuseTextureShader->SetVector3(ldUniform, lightDir.x, lightDir.y, lightDir.z);

    //Diffúz textúra shader #2
    _diffuseTextureShader2 = new Shader();
    if(!_diffuseTextureShader2->Load(DiffuseTextureVS, DiffuseTextureFS2)){
        cout << "Diffuse texture shader hiba!"<<endl;
        Pong3D::GlobalError = true;
        return;
    }

    _diffuseTextureShader2->Apply();
    _diffuseTextureShader2->BindLocation("vPos", 0);
    _diffuseTextureShader2->BindLocation("vNorm", 1);
    _diffuseTextureShader2->BindLocation("vUV", 2);
    _difTexBaseColor2 = _diffuseTextureShader2->GetUniform("BaseColor");
    _difTexSampler2 = _diffuseTextureShader2->GetUniform("Texture");
    _difTexPV2 = _diffuseTextureShader2->GetUniform("PV");
    _difTexW2 = _diffuseTextureShader2->GetUniform("W");
    ldUniform = _diffuseTextureShader2->GetUniform("LD");
    _diffuseTextureShader2->SetVector3(ldUniform, lightDir.x, lightDir.y, lightDir.z);

    //Égbolt séder
    _skyboxShader = new Shader();
    if(!_skyboxShader->Load(SkyboxVS, SkyboxFS)){
        cout << "Skybox shader hiba!"<<endl;
        Pong3D::GlobalError = true;
        return;
    }

    _skyboxShader->Apply();
    _skyboxShader->BindLocation("vPos", 0);
    _skyboxShader->BindLocation("vNorm", 1);
    _skyboxSampler = _skyboxShader->GetUniform("TexCube");
    _skyboxPV = _skyboxShader->GetUniform("PV");
    _skyboxW = _skyboxShader->GetUniform("W");
}

Game::~Game()
{
    SoundManager::Cleanup();

    delete _pongLogo;
    _pongLogo = NULL;

    delete _menuBG;
    _menuBG = NULL;

    delete [] _pButtons;
    _pButtons = NULL;

    delete _botBTN;
    _botBTN = NULL;

    delete [] _numbers;
    _numbers = NULL;

    delete [] _playerPoints;
    _playerPoints = NULL;

    delete [] _pButtonHelp;
    _pButtonHelp = NULL;

    delete _skybox;

    PlaneRenderer::Cleanup();
    CubeRenderer::Cleanup();
    UVPlane::Cleanup();
    SphereRenderer::Cleanup();

    for(BB* &b : Map)
        delete b;

    for(Paddle* &p : Pads)
        delete p;

    for(Ball* &b : Ballz)
        delete b;
}

/*====TEXTÚRA BETÖLTöK=====*/
/*void Game::LoadTexture(Texture2D* tex, string file)
{
    if(!tex->Load(file))
    {
        Pong3D::GlobalError = true;
    }
}*/

void Game::LoadTextureMem(Texture2D* tex, DataBlock data)
{
    if(!tex->LoadMem(data))
    {
        Pong3D::GlobalError = true;
    }
}

/*====ALAP UPDATE & RENDER======*/
void Game::Update()
{
    if(Keyboard::IsKeyDown(SDL_SCANCODE_ESCAPE))
    {
        if(_state == State::Gameplay)
        {
            if(_escTime >= 1.0f)
                OnFinished(false);
        }
        else
        {
            _pong->Exit();
        }
    }
    else
    {
        if(_escTime > 0.0f)
            _escTime = 0.0f;
    }

    if(Keyboard::IsKeyDown(SDL_SCANCODE_F11)){
        if(!_fsButton){
            _fsButton = true;

            _pong->GetGFX()->ToggleFullscreen();
        }
    }
    else
    {
        _fsButton = false;
    }

    if(Keyboard::IsKeyDown(SDL_SCANCODE_F1)){
        if(!_fpsButton){
            _fpsButton = true;
            _pong->SetTargetFPS(0);
        }
    }
    else if(Keyboard::IsKeyDown(SDL_SCANCODE_F2)){
        if(!_fpsButton){
            _fpsButton = true;
            _pong->SetTargetFPS(30);
        }
    }
    else if(Keyboard::IsKeyDown(SDL_SCANCODE_F3)){
       if(!_fpsButton){
            _fpsButton = true;
            _pong->SetTargetFPS(60);
        }
    }
    else
        _fpsButton = false;

    if(Keyboard::IsKeyDown(SDL_SCANCODE_F5)){
        camRotation += 45 * Pong3D::ElapsedTime;
    }
    if(Keyboard::IsKeyDown(SDL_SCANCODE_F6)){
        camRotation -= 45 * Pong3D::ElapsedTime;
    }

    if(_menuFade){
        if(_menuAlpha > _menuTargetAlpha){
            _menuAlpha -= Pong3D::ElapsedTime * 4;
            if(_menuAlpha <= 0){
                _menuAlpha = 0.0f;
                _menuFade = false;

                if(_gameStart){
                    _state = Gameplay;
                    _gameStart = false;
                }
            }
        }else{
            _menuAlpha += Pong3D::ElapsedTime * 4;
            if(_menuAlpha >= 1){
                _menuAlpha = 1.0f;
                _menuFade = false;
            }
        }
    }

    if(_state != State::Gameplay)
    {
        UpdateMenu(_state == State::Finish);
    }

    UpdateGame();
}

void Game::Render()
{
    DrawGame();

    if(_state != State::Gameplay)
    {
        DrawMenu(_state == State::Finish);
    }
}

/*=====MENÜ=====*/
void Game::UpdateMenu(bool board)
{
    if(board)
    {
        if(Mouse::LeftPress && Mouse::HitTest(menuX + (menuW / 2) - (_okBTN->Width / 2), menuY + menuH - (_okBTN->Height * 1.5f), _okBTN->Width, _okBTN->Height)){
           _state = State::Menu;
        }
    }
    else
    {
        if(Mouse::LeftPress)
        {
            if(Mouse::HitTest(menuX + menuW - _xBTN->Width * 2, btnsY, _xBTN->Width, _xBTN->Height)){
                _p1Ready = false;
                _p1Bot = false;
            }
            if(Mouse::HitTest(menuX + menuW - _xBTN->Width * 2, btnsY + btnPadding * 2, _xBTN->Width, _xBTN->Height)){
                _p2Ready = false;
                _p2Bot = false;
            }
            if(Mouse::HitTest(menuX + menuW - _xBTN->Width * 2, btnsY + btnPadding * 4, _xBTN->Width, _xBTN->Height)){
                _p3Ready = false;
                _p3Bot = false;
            }
            if(Mouse::HitTest(menuX + menuW - _xBTN->Width * 2, btnsY + btnPadding * 6, _xBTN->Width, _xBTN->Height)){
                _p4Ready = false;
                _p4Bot = false;
            }

            if(Mouse::HitTest(menuX + menuW - _botBTN->Width * 2.5f, btnsY, _botBTN->Width, _botBTN->Height)){
                _p1Ready = false;
                _p1Bot = true;
            }
            if(Mouse::HitTest(menuX + menuW - _botBTN->Width * 2.5f, btnsY + btnPadding * 2, _botBTN->Width, _botBTN->Height)){
                _p2Ready = false;
                _p2Bot = true;
            }
            if(Mouse::HitTest(menuX + menuW - _botBTN->Width * 2.5f, btnsY + btnPadding * 4, _botBTN->Width, _botBTN->Height)){
                _p3Ready = false;
                _p3Bot = true;
            }
            if(Mouse::HitTest(menuX + menuW - _botBTN->Width * 2.5f, btnsY + btnPadding * 6, _botBTN->Width, _botBTN->Height)){
                _p4Ready = false;
                _p4Bot = true;
            }

            _gameReady = CanGameStart();

            if(_gameReady && Mouse::HitTest(menuX + (menuW / 2) - (_startBTN->Width / 2), menuY + menuH - (_startBTN->Height * 1.5f), _startBTN->Width, _startBTN->Height)){
                SetupGame();

                _menuTargetAlpha = 0.0f;
                _menuFade = true;
                _gameStart = true;
            }
        }

        if(Keyboard::IsKeyDown(SDL_SCANCODE_LEFT) && Keyboard::IsKeyDown(SDL_SCANCODE_RIGHT)){
            _p1Ready = true;
            _p1Bot = false;

            _gameReady = CanGameStart();
        }
        if(Keyboard::IsKeyDown(SDL_SCANCODE_A) && Keyboard::IsKeyDown(SDL_SCANCODE_S)){
            _p2Ready = true;
            _p2Bot = false;

            _gameReady = CanGameStart();
        }
        if(Keyboard::IsKeyDown(SDL_SCANCODE_B) && Keyboard::IsKeyDown(SDL_SCANCODE_N)){
            _p3Ready = true;
            _p3Bot = false;

            _gameReady = CanGameStart();
        }
        if(Keyboard::IsKeyDown(SDL_SCANCODE_O) && Keyboard::IsKeyDown(SDL_SCANCODE_P)){
            _p4Ready = true;
            _p4Bot = false;

            _gameReady = CanGameStart();
        }
    }
}

void Game::DrawTextures(Texture2D* a, Texture2D* b, float destX, float destY, float destW)
{
    float fullW = a->Width + b->Width;
    float nameWidth = a->Width / fullW;
    float winzWidth = b->Width / fullW;

    nameWidth = nameWidth * destW;
    winzWidth = winzWidth * destW;

    float textHeight = (nameWidth / a->Width) * a->Height;

    _batch->DrawTexture(a, destX, destY, nameWidth, textHeight, 0, 0, 0, 1.0f);
    _batch->DrawTexture(b, destX + nameWidth, destY, winzWidth, textHeight, 0, 0, 0, 1.0f);
}

void Game::DrawMenu(bool board)
{
    _batch->Begin();
    _batch->SetColor(1,1,1,_menuAlpha);
    _batch->DrawTexture(_pongLogo, (1280 / 2 - _pongLogo->Width / 2), (720 / 6 - _pongLogo->Height / 2),
                        _pongLogo->Width, _pongLogo->Height, _pongLogo->Width / 2, _pongLogo->Height / 2, sin(Pong3D::TotalRunTime * 3) * 2.5f, ((sin(Pong3D::TotalRunTime * 5) + 1) / 40) + 1);

    _batch->DrawTexture(_menuBG, menuX, menuY, menuW, menuH, 0.0f, 0.0f, 0.0f, 1.0f);

    if(board)
    {
        _batch->SetColor(1,1,1,_menuAlpha);

        //Gyöztes szöveg
        Texture2D* winner = &_pNames[_finishOrder[0]];
        _batch->DrawTexture(winner, menuX, menuY, nameWidth, textHeight, 0, 0, 0, 1.0f);
        _batch->DrawTexture(_winz, menuX + nameWidth, menuY, winzWidth, textHeight, 0, 0, 0, 1.0f);

        //Sorrend
        for(int x = 0;x<_finishPlayers;x++)
        {
            int linePlayer = _finishOrder[x];

            Texture2D* pName = &_pNames[linePlayer];
            int drawY = lineY + (pName->Height + winPadding) * x;

            _batch->DrawTexture(pName, lineX, drawY, pName->Width, pName->Height, 0, 0, 0, 1.0f);

            int pp = _finishPoints[linePlayer];
            Texture2D* num = &_numbers[pp % 10];

            _batch->DrawTexture(num, lineX + pName->Width + 15 + num->Width + 5, drawY, winnumW, winnumH, 0, 0, 0, 1.0f);
            if(pp > 9)
            {
                num = &_numbers[pp / 10];
                _batch->DrawTexture(num, lineX + pName->Width + 15, drawY, winnumW, winnumH, 0, 0, 0, 1.0f);
            }
        }

        _batch->SetColor(1,1,1,_menuAlpha);
        _batch->DrawTexture(_okBTN, menuX + (menuW / 2) - (_okBTN->Width / 2), menuY + menuH - (_okBTN->Height * 1.5f), _okBTN->Width, _okBTN->Height,
                            0, 0, 0, 1.0f);
    }
    else
    {
        _batch->SetColor(1.0f, _menuAlpha);
        _batch->DrawTexture(_menu, menuX + (menuW / 2.0f) - (_menu->Width / 2.0f), menuY + 15, _menu->Width, _menu->Height, 0, 0, 0, 1.0f);

        _batch->SetColor(1,(_gameReady ? 1.0f : 0.25f),1,_menuAlpha);
        _batch->DrawTexture(_startBTN, menuX + (menuW / 2) - (_startBTN->Width / 2), menuY + menuH - (_startBTN->Height * 1.5f), _startBTN->Width, _startBTN->Height,
                            0, 0, 0, 1.0f);

        _batch->SetColor((_p1Ready ? 1 : 0.5f), _menuAlpha);
        _batch->DrawTexture((&_pButtons[0]), btnsX, btnsY, (&_pButtons[0])->Width, (&_pButtons[0])->Height, 0, 0, 0, 1.0f);
        _batch->SetColor((_p1Bot ? 1 : 0.5f), _menuAlpha);
        _batch->DrawTexture(_botBTN, menuX + menuW - _botBTN->Width * 2.5f, btnsY, _botBTN->Width, _botBTN->Height, 0, 0, 0, 1.0f);
        _batch->SetColor(1,1,1,_menuAlpha);
        _batch->DrawTexture(_xBTN, menuX + menuW - _xBTN->Width * 2, btnsY, _xBTN->Width, _xBTN->Height, 0, 0, 0, 1.0f);

        _batch->SetColor((_p2Ready ? 1 : 0.5f), _menuAlpha);
        _batch->DrawTexture((&_pButtons[1]), btnsX, btnsY + btnPadding * 2, (&_pButtons[1])->Width, (&_pButtons[1])->Height, 0, 0, 0, 1.0f);
        _batch->SetColor((_p2Bot ? 1 : 0.5f), _menuAlpha);
        _batch->DrawTexture(_botBTN, menuX + menuW - _botBTN->Width * 2.5f, btnsY + btnPadding * 2, _botBTN->Width, _botBTN->Height, 0, 0, 0, 1.0f);
        _batch->SetColor(1,1,1,_menuAlpha);
        _batch->DrawTexture(_xBTN, menuX + menuW - _xBTN->Width * 2, btnsY + btnPadding * 2, _xBTN->Width, _xBTN->Height, 0, 0, 0, 1.0f);

        _batch->SetColor((_p3Ready ? 1 : 0.5f), _menuAlpha);
        _batch->DrawTexture((&_pButtons[2]), btnsX, btnsY + btnPadding * 4, (&_pButtons[2])->Width, (&_pButtons[2])->Height, 0, 0, 0, 1.0f);
        _batch->SetColor((_p3Bot ? 1 : 0.5f), _menuAlpha);
        _batch->DrawTexture(_botBTN, menuX + menuW - _botBTN->Width * 2.5f, btnsY + btnPadding * 4, _botBTN->Width, _botBTN->Height, 0, 0, 0, 1.0f);
        _batch->SetColor(1,1,1,_menuAlpha);
        _batch->DrawTexture(_xBTN, menuX + menuW - _xBTN->Width * 2, btnsY + btnPadding * 4, _xBTN->Width, _xBTN->Height, 0, 0, 0, 1.0f);

        _batch->SetColor((_p4Ready ? 1 : 0.5f), _menuAlpha);
        _batch->DrawTexture((&_pButtons[3]), btnsX, btnsY + btnPadding * 6, (&_pButtons[3])->Width, (&_pButtons[3])->Height, 0, 0, 0, 1.0f);
        _batch->SetColor((_p4Bot ? 1 : 0.5f), _menuAlpha);
        _batch->DrawTexture(_botBTN, menuX + menuW - _botBTN->Width * 2.5f, btnsY + btnPadding * 6, _botBTN->Width, _botBTN->Height, 0, 0, 0, 1.0f);
        _batch->SetColor(1,1,1,_menuAlpha);
        _batch->DrawTexture(_xBTN, menuX + menuW - _xBTN->Width * 2, btnsY + btnPadding * 6, _xBTN->Width, _xBTN->Height, 0, 0, 0, 1.0f);
    }

    _batch->End();
}

/*=====JÁTÉK=====*/
bool Game::CanGameStart()
{
    bool p1 = (_p1Ready || _p1Bot);
    bool p2 = (_p2Ready || _p2Bot);
    bool p3 = (_p3Ready || _p3Bot);
    bool p4 = (_p4Ready || _p4Bot);

    return ((p1 && p2 && !p3 && !p4) || (p1 && p2 && p3 && !p4) || (p1 && p2 && p3 && p4));
}

void Game::BuildMap(int pc)
{
    float ms = MapSize;

    float lineL = MapSize - wallSize - cornerSize;

    mapOffsetX = 0.0f;

    switch(pc){
    case 4:
        {
            Map[0]->SetAnimScale(1.0f); //Felsö fal
            Map[1]->SetAnimScale(1.0f); //Alsó fal
            Map[2]->SetAnimScale(1.0f); //Jobb nagy fal
            Map[3]->SetAnimScale(1.0f); //Bal nagy fal

            Map[4]->SetAnimScale(1.0f); //Jobb felsö sarok
            Map[5]->SetAnimScale(1.0f); //Bal felsö sarok
            Map[6]->SetAnimScale(1.0f); //Jobb alsó sarok
            Map[7]->SetAnimScale(1.0f); //Bal alsó sarok

            Map[8]->SetAnimScale(0.0f); //Felsö kis fal
            Map[9]->SetAnimScale(0.0f); //Alsó kis fal
            Map[10]->SetAnimScale(0.0f); //Jobb kis fal
            Map[11]->SetAnimScale(0.0f); //Bal kis fal

            Map[12]->SetAnimScale(0.0f); //Jobb fura fal
            Map[13]->SetAnimScale(0.0f); //Bal fura fal
            Map[14]->SetAnimScale(0.0f); //Alsó fura fal

            mapRenderX = MapSize;
            mapRenderZ = MapSize;

            _p1Playing = _p2Playing = _p3Playing = _p4Playing = true;
        }
        break;
    case 3:
        {
            Map[0]->SetAnimScale(1.0f); //Felsö fal
            Map[1]->SetAnimScale(0.0f); //Alsó fal
            Map[2]->SetAnimScale(0.0f); //Jobb nagy fal
            Map[3]->SetAnimScale(0.0f); //Bal nagy fal

            Map[4]->SetAnimScale(1.0f); //Jobb felsö sarok
            Map[5]->SetAnimScale(1.0f); //Bal felsö sarok
            Map[6]->SetAnimScale(0.0f); //Jobb alsó sarok
            Map[7]->SetAnimScale(0.0f); //Bal alsó sarok

            Map[8]->SetAnimScale(0.0f); //Felsö kis fal
            Map[9]->SetAnimScale(0.0f); //Alsó kis fal
            Map[10]->SetAnimScale(0.0f); //Jobb kis fal
            Map[11]->SetAnimScale(0.0f); //Bal kis fal

            Map[12]->SetAnimScale(1.0f); //Jobb fura fal
            Map[13]->SetAnimScale(1.0f); //Bal fura fal
            Map[14]->SetAnimScale(1.0f); //Alsó fura fal

            mapRenderX = lineL / 2 + MapSize / 2;
            mapRenderZ = MapSize;
            mapOffsetX = -lineL / 2;

            _p2Playing = _p2Playing = _p3Playing = true;
            _p4Playing = false;
        }
        break;
    case 2:
        {
            Map[0]->SetAnimScale(0.0f); //Felsö fal
            Map[1]->SetAnimScale(0.0f); //Alsó fal
            Map[2]->SetAnimScale(0.0f); //Jobb nagy fal
            Map[3]->SetAnimScale(0.0f); //Bal nagy fal

            Map[4]->SetAnimScale(0.0f); //Jobb felsö sarok
            Map[5]->SetAnimScale(0.0f); //Bal felsö sarok
            Map[6]->SetAnimScale(0.0f); //Jobb alsó sarok
            Map[7]->SetAnimScale(0.0f); //Bal alsó sarok

            Map[8]->SetAnimScale(1.0f); //Felsö kis fal
            Map[9]->SetAnimScale(1.0f); //Alsó kis fal
            Map[10]->SetAnimScale(1.0f); //Jobb kis fal
            Map[11]->SetAnimScale(1.0f); //Bal kis fal

            Map[12]->SetAnimScale(0.0f); //Jobb fura fal
            Map[13]->SetAnimScale(0.0f); //Bal fura fal
            Map[14]->SetAnimScale(0.0f); //Alsó fura fal

            mapRenderX = lineL;
            mapRenderZ = MapSize;

            _p2Playing = _p2Playing = true;
            _p3Playing = _p4Playing = false;
        }
        break;
    }

    Pads[0]->Animate = true;
    Pads[0]->AnimTargetScale = (_p1Playing ? 1.0f : 0.0f);
    Pads[0]->AnimateSize = true;
    Pads[0]->TargetSize = BasePadSize;

    Pads[1]->Animate = true;
    Pads[1]->AnimTargetScale = (_p2Playing ? 1.0f : 0.0f);
    Pads[1]->AnimateSize = true;
    Pads[1]->TargetSize = BasePadSize;

    Pads[2]->Animate = true;
    Pads[2]->AnimTargetScale = (_p3Playing ? 1.0f : 0.0f);
    Pads[2]->AnimateSize = true;
    Pads[2]->TargetSize = BasePadSize;

    Pads[3]->Animate = true;
    Pads[3]->AnimTargetScale = (_p4Playing ? 1.0f : 0.0f);
    Pads[3]->AnimateSize = true;
    Pads[3]->TargetSize = BasePadSize;
}

void Game::SetupGame()
{
    int playerCount = 0;
    if(_p1Ready || _p1Bot)
    {
        playerCount++;
        _playerPoints[0] = 0;
    }
    else
    {
        _playerPoints[0] = -1;
    }
    if(_p2Ready || _p2Bot)
    {
        playerCount++;
        _playerPoints[1] = 0;
    }
    else
    {
        _playerPoints[1] = -1;
    }
    if(_p3Ready || _p3Bot)
    {
        playerCount++;
        _playerPoints[2] = 0;
    }
    else
    {
        _playerPoints[2] = -1;
    }
    if(_p4Ready || _p4Bot)
    {
        playerCount++;
        _playerPoints[3] = 0;
    }
    else
    {
        _playerPoints[3] = -1;
    }

    _targetPoints = 10;
    //if(playerCount > 2)
        //_targetPoints += 10;

    BuildMap(playerCount);

    Pads[0]->Bot = _p1Bot;
    Pads[1]->Bot = _p2Bot;
    Pads[2]->Bot = _p3Bot;
    Pads[3]->Bot = _p4Bot;

    _countdown = true;
    _initialCountdown = true;
    _countdownValue = 3.0f;

    for(int x = 0;x<4;x++)
        Pads[x]->GotoCenter = true;

    for(Ball* &b : Ballz)
        delete b;
    Ballz.clear();

    for(Powerup* &p : Powerups)
        delete p;
    Powerups.clear();

    Ballz.push_back(new Ball(BaseBallSize, mapH / 2));
    Ballz[0]->SpawnBall((rand() + 1) % playerCount, MapSize);

    PlayerCount = playerCount;
}

void Game::UpdateGame()
{
    bool canMove = ((_state == State::Gameplay && !_countdown) || (_state == State::Menu && !_gameStart) || (_state == State::Finish));
    for(Paddle* p : Pads)
    {
        p->Update(this, canMove);
    }

    if(canMove)
    {
        for(Ball* b : Ballz)
        {
            if(b->NewBall)
                b->NewBall = false;
            b->Update(this);
        }
    }

    if(_state == State::Gameplay)
    {
        if(_countdown)
        {
            _countdownValue -= Pong3D::ElapsedTime;
            if(_countdownValue <= 0.0f)
            {
                _countdown = false;
                _countdownValue = 0.0f;
                _initialCountdown = false;

                for(int x = 0;x<4;x++)
                    Pads[x]->GotoCenter = false;
            }
        }
    }

    if(!_countdown)
    {
        if(_powerupSpawnTime < 5.0f)
        {
            _powerupSpawnTime += Pong3D::ElapsedTime;
        }
        else
        {
            _powerupSpawnTime = 0.0f;

            if(Powerups.size() < 3)
            {
                float xRand;
                float zRand;

                do
                {
                    xRand = (((float)rand() / (float)RAND_MAX) - 0.5f) * 2.0f * (mapRenderX / 2);
                    zRand = (((float)rand() / (float)RAND_MAX) - 0.5f) * 2.0f * (mapRenderZ / 2);
                }while(IsPowerupNear(xRand,zRand));

                PowerupTarget trg = (PowerupTarget)(rand() % (int)PowerupTarget::TrgMax);
                PowerupType typ = (PowerupType)(rand() % (int)PowerupType::TypeMax);
                int ty = 0;
                do{
                    if(!Powerup::CanSpawn(this, typ, trg)){
                        trg = (PowerupTarget)(rand() % (int)PowerupTarget::TrgMax);
                        typ = (PowerupType)(rand() % (int)PowerupType::TypeMax);
                    }

                    ty++;
                }while(ty < 10);

                Powerups.push_back(new Powerup(xRand,0.2f,zRand,trg,typ));
            }
        }
    }
}

bool Game::IsPowerupNear(float x, float z)
{
    for(Powerup* &p : Powerups)
    {
        if(sqrt(pow(x-p->X,2)+pow(z-p->Z,2)) < Powerup::Radius * 2)
        {
            return true;
        }
    }

    return false;
}

void Game::DrawGame()
{
    if(abs(mapOffsetX - camLookX) > 0.1f)
        camLookX += (mapOffsetX - camLookX) * Pong3D::ElapsedTime;
    if(abs(mapRenderX - camRenderX) > 0.1f)
        camRenderX += (mapRenderX - camRenderX) * Pong3D::ElapsedTime;
    if(abs(mapRenderZ - camRenderZ) > 0.1f)
        camRenderZ += (mapRenderZ - camRenderZ) * Pong3D::ElapsedTime;

    //Nézet beállítások
    glm::mat4 p = glm::perspective(45.0f, ((float)_pong->GetGFX()->GetContextWidth() / (float)_pong->GetGFX()->GetContextHeight()), 1.0f, 150.0f);
    glm::mat4 v = glm::lookAt(glm::vec3(camRenderX * 1.5f + camLookX, 8, camRenderZ * 1.5f), glm::vec3(camLookX,-1,0), glm::vec3(0,1,0));
    v = glm::rotate(v, camRotation, glm::vec3(0,1,0));
    glm::mat4 pv = p*v;

    float lineL = MapSize - wallSize - cornerSize;
    float lineW = 0.1f;

    //Pálya megjelenítés
    {
        _diffuseColorShader->Apply();
        _diffuseColorShader->SetMatrix(_difPV, glm::value_ptr(pv));

        PlaneRenderer::BindVAO();

        //Padló
        _diffuseColorShader->SetVector3(_difBaseColor, 34.0f/255.0f, 200.0f/255.0f,34.0f/255.0f); //Pong zöld
        _diffuseColorShader->SetMatrix(_difW, glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(mapOffsetX, 0.0f, 0.0f)), glm::vec3(mapRenderX, 1.0f, mapRenderZ)));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);

        //Kapu vonalak
        _diffuseColorShader->SetVector3(_difBaseColor, 1,1,1);

        if(_p1Playing)
        {
            _diffuseColorShader->SetMatrix(_difW, glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.01f, -MapSize + wallSize + gateSize - lineW)), glm::vec3(lineL, 1, lineW)));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
        }

        if(_p2Playing)
        {
            _diffuseColorShader->SetMatrix(_difW, glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.01f, MapSize - wallSize - gateSize + lineW)), glm::vec3(lineL, 1, lineW)));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
        }

        if(_p3Playing)
        {
            _diffuseColorShader->SetMatrix(_difW, glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-MapSize + wallSize + gateSize - lineW, 0.01f, 0)), glm::vec3(lineW, 1, lineL)));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
        }

        if(_p4Playing)
        {
            _diffuseColorShader->SetMatrix(_difW, glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(MapSize - wallSize - gateSize + lineW, 0.01f, 0)), glm::vec3(lineW, 1, lineL)));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
        }

        PlaneRenderer::UnbindVAO();

        //Falak
        CubeRenderer::BindVAO();
        _diffuseColorShader->SetVector3(_difBaseColor, 0.9f, 0.9f, 0.9f);
        for(BB* &b : Map)
        {
            if(b->Animate)
            {
                if(b->AnimScale < b->AnimTargetScale)
                {
                    b->AnimScale += Pong3D::ElapsedTime * 7;
                    if(b->AnimScale >= 1.0f)
                    {
                        b->AnimScale = 1.0f;
                        b->Animate = false;
                    }
                }
                else if(b->AnimScale > b->AnimTargetScale)
                {
                    b->AnimScale -= Pong3D::ElapsedTime * 7;
                    if(b->AnimScale <= 0.0f)
                    {
                        b->AnimScale = 0.0f;
                        b->Animate = false;
                    }
                }
                else
                {
                    b->Animate = false;
                }
            }

            if(b->AnimScale > 0.0f)
                DrawCube(b,0,0,0,b->AnimScale);
        }

        //Ütök
        _diffuseColorShader->SetVector3(_difBaseColor, 0.6f, 0.6f, 0.6f);
        for(Paddle* &p : Pads)
        {
            p->Render(this);
        }

        CubeRenderer::UnbindVAO();

        SphereRenderer::BindVAO();
        //Labdák
        _diffuseColorShader->SetVector3(_difBaseColor, 1,1,1);
        for(Ball* &b : Ballz)
        {
            b->Render(this);
        }
        SphereRenderer::UnbindVAO();

        //Számok
        UVPlane::BindVAO();

        _diffuseTextureShader->Apply();
        _diffuseTextureShader->SetMatrix(_difTexPV, glm::value_ptr(pv));
        _diffuseTextureShader->SetVector3(_difTexBaseColor, 1, 1, 1);

        if(_state == State::Gameplay)
        {
            if(_p1Playing)
                DrawNumber(glm::vec3(lineL / 1.5f, 0, -MapSize / 1.75f), 0.0f, Pads[0]->AnimScale, _playerPoints[0]);

            if(_p2Playing)
                DrawNumber(glm::vec3(lineL / 1.5f, 0, MapSize / 1.75f), 0.0f, Pads[1]->AnimScale, _playerPoints[1]);

            if(_p3Playing)
                DrawNumber(glm::vec3(-MapSize / 1.75f, 0, lineL / 1.5f), 90.0f, Pads[2]->AnimScale, _playerPoints[2]);

            if(_p4Playing)
                DrawNumber(glm::vec3(MapSize / 1.75f, 0, lineL / 1.5f), 90.0f, Pads[3]->AnimScale, _playerPoints[3]);

            if(_initialCountdown)
            {
                float scl = (_countdownValue > 0.5f ? 1.0f : _countdownValue * 2.0f);

                if(_p1Playing && !_p1Bot)
                    DrawButtonHelp(glm::vec3(0, 0, -MapSize / 1.75f), 0.0f, scl, 0);
                if(_p2Playing && !_p2Bot)
                    DrawButtonHelp(glm::vec3(0, 0, MapSize / 1.75f), 0.0f, scl, 1);
                if(_p3Playing && !_p3Bot)
                    DrawButtonHelp(glm::vec3(-MapSize / 1.75f, 0, 0), 90.0f, scl, 2);
                if(_p4Playing && !_p4Bot)
                    DrawButtonHelp(glm::vec3(MapSize / 1.75f, 0, 0), 90.0f, scl, 3);
            }
        }

        //Powerup-ok
        if(Powerups.size() > 0)
        {
            _diffuseTextureShader2->Apply();
            _diffuseTextureShader2->SetMatrix(_difTexPV2, glm::value_ptr(pv));

            for(Powerup* &p : Powerups)
            {
                p->Update(this);

                switch(p->Target)
                {
                case PowerupTarget::All:
                    _diffuseTextureShader2->SetVector4(_difTexBaseColor2, 0.75f, 0.75f, 0.75f, p->Alpha);
                    break;
                case PowerupTarget::Self:
                    _diffuseTextureShader2->SetVector4(_difTexBaseColor2, 0.25f, 0.25f, 1, p->Alpha);
                    break;
                case PowerupTarget::Others:
                    _diffuseTextureShader2->SetVector4(_difTexBaseColor2, 1, 0.25f, 0.25f, p->Alpha);
                    break;
                }
                _diffuseTextureShader2->SetSampler(_difTexSampler2, 0, (&_powerupImages[(int)p->Type])->GetID());
                DrawUVPlane(p->Bounds, p->X, p->Y, p->Z, p->Scale);
            }
        }

        UVPlane::UnbindVAO();

        //Égbol
        SphereRenderer::BindVAO();

        _skyboxShader->Apply();
        _skyboxShader->SetSampler(_skyboxSampler, 0, _skybox->GetID());
        _skyboxShader->SetMatrix(_skyboxPV, glm::value_ptr(pv));
        _skyboxShader->SetMatrix(_skyboxW, glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -7.5f, 0.0f)), glm::vec3(-20.0f)));

        glDrawElements(GL_TRIANGLES, SphereRenderer::IndexLength, GL_UNSIGNED_SHORT, NULL);

        SphereRenderer::UnbindVAO();
    }

    if(_countdown)
    {
        _batch->Begin();
        _batch->SetColor(1.0f, 1.0f);
        _batch->DrawTexture(&_numbers[(int)ceil(_countdownValue)], (1280 / 2) - 25, (720 / 2) - 50, 50, 100, 0, 0, 0, 1.0f);
        _batch->End();
    }
}

/*====ESZKÖZÖK======*/
void Game::DrawSphere(BB* bb, float x, float y, float z, float scale)
{
    glm::mat4 m = bb->GetWorldMatrix();
    glm::mat4 wm = glm::translate(glm::mat4(1.0f), glm::vec3(x,y,z));
    glm::mat4 anims = glm::scale(glm::mat4(1.0f), glm::vec3(scale));

    glm::mat4 w = wm * m * anims;

    _diffuseColorShader->SetMatrix(_difW, w);
    glDrawElements(GL_TRIANGLES, SphereRenderer::IndexLength, GL_UNSIGNED_SHORT, NULL);
}

void Game::DrawCube(BB* bb, float x, float y, float z, float scale)
{
    glm::mat4 m = bb->GetWorldMatrix();
    glm::mat4 wm = glm::translate(glm::mat4(1.0f), glm::vec3(x,y,z));
    glm::mat4 anims = glm::scale(glm::mat4(1.0f), glm::vec3(scale));

    glm::mat4 w = wm * m * anims;

    _diffuseColorShader->SetMatrix(_difW, w);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, NULL);
}

void Game::DrawCube(BB* bb, float x, float y, float z, float sx, float sz)
{
    glm::mat4 m = bb->GetWorldMatrix();
    glm::mat4 wm = glm::translate(glm::mat4(1.0f), glm::vec3(x,y,z));
    glm::mat4 anims = glm::scale(glm::mat4(1.0f), glm::vec3(sx, 1.0f, sz));

    glm::mat4 w = wm * m * anims;

    _diffuseColorShader->SetMatrix(_difW, w);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, NULL);
}

void Game::DrawUVPlane(BB* bb, float x, float y, float z, float scale)
{
    glm::mat4 m = bb->GetWorldMatrixPlane();
    glm::mat4 wm = glm::translate(glm::mat4(1.0f), glm::vec3(x,y,z));
    glm::mat4 anims = glm::scale(glm::mat4(1.0f), glm::vec3(scale));

    glm::mat4 w = wm * m * anims;

    _diffuseTextureShader2->SetMatrix(_difTexW2, w);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
}

void Game::DrawNumber(glm::vec3 pos, float rot, float scale, int num)
{
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f * scale, 1, 0.5f * scale));
    glm::mat4 basePos = glm::translate(glm::mat4(1.0f), glm::vec3(0 + pos.x,0.05f + pos.y,0 + pos.z));
    glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), rot, glm::vec3(0,1,0));

    _diffuseTextureShader->SetMatrix(_difTexW, basePos * rotMat * scaleMat);
    _diffuseTextureShader->SetSampler(_difTexSampler, 0, (&_numbers[num % 10])->GetID());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);

    if(num > 9)
    {
        glm::mat4 shiftPos = glm::translate(glm::mat4(1.0f), glm::vec3(-0.75,0,0));

        _diffuseTextureShader->SetMatrix(_difTexW, basePos * rotMat * shiftPos * scaleMat);
        _diffuseTextureShader->SetSampler(_difTexSampler, 0, (&_numbers[num / 10])->GetID());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
    }
}

void Game::DrawButtonHelp(glm::vec3 pos, float rot, float scale, int player)
{
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(0.75f * scale, 1, 0.5f * scale));
    glm::mat4 basePos = glm::translate(glm::mat4(1.0f), glm::vec3(0 + pos.x,0.05f + pos.y,0 + pos.z));
    glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), rot, glm::vec3(0,1,0));

    _diffuseTextureShader->SetMatrix(_difTexW, basePos * rotMat * scaleMat);
    _diffuseTextureShader->SetSampler(_difTexSampler, 0, (&_pButtonHelp[player])->GetID());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
}

/*====JTÁÉK ESEMÉNYEK======*/
void Game::OnBallRemoved(int lt, int tag)
{
    if(lt > -1)
    {
        _playerPoints[lt]++;
        if(_targetPoints > 0 && _playerPoints[lt] >= _targetPoints)
        {
            OnFinished(true);
        }
    }

    if(Ballz.size() == 0)
    {
        if(_state != State::Gameplay)
        {
            Ballz.push_back(new Ball(BaseBallSize, mapH / 2.0f));
            Ballz[0]->RandomDir();
        }
        else
        {
            if(lt > -1 && _targetPoints > 0 && _playerPoints[lt] >= _targetPoints)
            {
                OnFinished(true);
            }

            _countdown = true;
            _countdownValue = 3.0f;
            SwapButtons = false;
            for(int x = 0;x<4;x++)
            {
                Pads[x]->GotoCenter = true;
                Pads[x]->TargetSize = BasePadSize;
                Pads[x]->AnimateSize = true;
            }
            _powerupSpawnTime = 0.0f;

            Ballz.push_back(new Ball(BaseBallSize, mapH / 2.0f));
            Ballz[0]->SpawnBall(tag, MapSize);
        }
    }
}

void Game::OnFinished(bool board)
{
    if(board)
    {
        _finishPlayers = PlayerCount;

        //Játékos sorrend
        for(int x = 0;x<4;x++)
        {
            _finishOrder[x] = x;
            _finishPoints[x] = _playerPoints[x];
        }

        //Bubis rendezés
        for(int a = 3; a > 0; a--)
        {
            for(int b = 0;b<a;b++)
            {
                if(_playerPoints[b]<_playerPoints[b+1])
                {
                    int tmp = _finishOrder[b];
                    _finishOrder[b] = _finishOrder[b+1];
                    _finishOrder[b+1] = tmp;
                }
            }
        }

        _state = State::Finish;
    }
    else
    {
        _countdown = false;
        _initialCountdown = false;

        _state = State::Menu;

        for(Ball* &b : Ballz)
            delete b;
        Ballz.clear();
    }

    //Botos háttér
    PlayerCount = 4;
    BuildMap(4); //Alap pálya

    _p1Playing = _p2Playing = _p3Playing = _p4Playing = true;
    for(int x = 0;x<4;x++)
    {
        Pads[x]->Bot = true;
        Pads[x]->Animate = true;
        Pads[x]->AnimTargetScale = 1.0f;
        Pads[x]->TargetSize = BasePadSize;
        Pads[x]->AnimateSize = true;

        _playerPoints[x] = 0;
    }

    _targetPoints = 0;

    Ballz.push_back(new Ball(BaseBallSize, mapH / 2.0f));
    Ballz[0]->RandomDir();

    _menuFade = true;
    _menuTargetAlpha = 1.0f;
}
