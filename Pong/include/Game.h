#ifndef GAME_H
#define GAME_H

#include <Texture2D.h>
#include <TextureCube.h>
#include <SpriteBatch.h>
#include <Shader.h>
#include <string>
#include <vector>
#include <BB.h>
#include <Paddle.h>
#include <Ball.h>
#include <Powerup.h>
#include <DataBlock.h>

using namespace std;

class Pong3D;
class Paddle;
class Ball;
class Powerup;

enum State{
    Menu,
    Gameplay,
    Finish
};

class Game
{
    public:
        Game(Pong3D* pong);
        virtual ~Game();

        //void LoadTexture(Texture2D* tex, string file);
        void LoadTextureMem(Texture2D* tex, DataBlock data);

        //Alap Update & Render
        void Update();
        void Render();

        //Rajzol� ezsk�z cuccosok
        void DrawSphere(BB* bb, float x, float y, float z, float scale);
        void DrawCube(BB* bb, float x, float y, float z, float scale);
        void DrawCube(BB* bb, float x, float y, float z, float scaleX, float scaleZ);
        void DrawUVPlane(BB* bb, float x, float y, float z, float scale);
        void DrawNumber(glm::vec3 pos, float rot, float scale, int number);
        void DrawButtonHelp(glm::vec3 pos, float rot, float scale, int player);

        //Men�
        void UpdateMenu(bool board);
        void DrawMenu(bool board);
        bool CanGameStart();
        void DrawTextures(Texture2D* a, Texture2D* b, float destX, float destY, float destW);

        //J�t�k
        void SetupGame();

        void UpdateGame();
        void DrawGame();

        bool IsPowerupNear(float x, float z); //Megmondja, hogy van e a ponthoz k�zel m�sik powerup
        void BuildMap(int playerCount);

        //Vektorok, amelyek mindenf�le j�t�k objektumokat t�rolnak
        vector<Ball*> Ballz;
        vector<BB*> Map;
        vector<Paddle*> Pads;
        vector<Powerup*> Powerups;

        //Alap j�t�k �rt�kek
        static float MapSize;
        static float BasePadSize;
        static float BaseBallSize;
        static float BaseBallSpeed;
        static float BaseCameraRotationSpeed;

        //J�t�k esem�nyek
        void OnBallRemoved(int lt, int wallTag);
        void OnFinished(bool board);

        int PlayerCount; //J�t�kos sz�m
        bool SwapButtons; //Powerup hat�s
    private:
        Pong3D* _pong;
        SpriteBatch* _batch;

        State _state; //J�t�k �llapot: men�, j�t�k, eredm�ny

        //Text�r�k
        Texture2D* _pongLogo;
        Texture2D* _menuBG;
        Texture2D* _menu;
        Texture2D* _startBTN;
        Texture2D* _okBTN;
        Texture2D* _pButtons;
        Texture2D* _botBTN;
        Texture2D* _xBTN;
        Texture2D* _pButtonHelp;
        Texture2D* _pNames;
        Texture2D* _winz;
        Texture2D* _powerupImages;
        Texture2D* _numbers;
        TextureCube* _skybox;

        bool _fpsButton = false; //Jelzi, hogy valamelyik FPS gomb le van-e nyomva (f1, f2 vagy f3)

        //Men� �tl�tsz�s�g anim�ci�
        bool _menuFade = false;
        float _menuAlpha = 1.0f;
        float _menuTargetAlpha = 0.0f;

        //J�t�kos �llapotok (lehetne t�mbbe is t�rolni, de �gy siker�lt)
        bool _p1Ready = true;
        bool _p1Bot = false;
        bool _p1Playing = false;

        bool _p2Ready = false;
        bool _p2Bot = true;
        bool _p2Playing = false;

        bool _p3Ready = false;
        bool _p3Bot = false;
        bool _p3Playing = false;

        bool _p4Ready = false;
        bool _p4Bot = false;
        bool _p4Playing = false;

        //J�t�k �llapot jelz�k (hossz� �-t nem �rhatok mert ragel a C::B)
        bool _gameReady = true;
        bool _gameStart = false;

        //Men� poz�ci�k a k�pen
        int menuX, menuY, menuW, menuH;
        int btnsX, btnsY, btnPadding;

        float nameWidth;
        float winzWidth;
        float textHeight;

        int lineY;
        float lineW;
        float lineX;
        int winPadding;
        float winnumH;
        float winnumW;

        //Visszasz�mol�shoz kell� v�ltoz�k
        bool _initialCountdown;
        bool _countdown;
        float _countdownValue;

        //Shader-ek
        Shader* _diffuseColorShader;
        unsigned int _difBaseColor;
        unsigned int _difPV;
        unsigned int _difW;

        Shader* _diffuseTextureShader;
        unsigned int _difTexBaseColor;
        unsigned int _difTexSampler;
        unsigned int _difTexPV;
        unsigned int _difTexW;

        Shader* _diffuseTextureShader2;
        unsigned int _difTexBaseColor2;
        unsigned int _difTexSampler2;
        unsigned int _difTexPV2;
        unsigned int _difTexW2;

        Shader* _skyboxShader;
        unsigned int _skyboxSampler;
        unsigned int _skyboxPV;
        unsigned int _skyboxW;

        //Sz�molja, hogy mennyi ideje van lenyomva az Esc billenty�
        float _escTime;

        //J�t�kos pontsz�m v�ltoz�k
        int* _playerPoints; //J�t�kosok pontsz�mai
        int _targetPoints; //C�l pontsz�m
        int* _finishOrder; //J�t�kos azonos�t�k sorrendbe a pontok alapj�n
        int* _finishPoints;
        int _finishPlayers; //Megadja, hogy h�nyan j�tszottak a k�rben (kell a pontsz�m megjelen�t�nek)

        float _powerupSpawnTime; //Ez sz�molja, hogy mikor kell �j powerup-ot l�trehozni

        //P�lya inf�k
        float wallSize;
        float mapH;
        float cornerSize;
        float gateSize;

        //P�lya �s kamera inf�k
        float mapRenderX;
        float mapRenderZ;
        float mapOffsetX;
        float camLookX;
        float camRenderX;
        float camRenderZ;
        float camRotationSpeed;

        //Shader k�dok
        static string DiffuseColorVS;
        static string DiffuseColorFS;
        static string DiffuseTextureVS;
        static string DiffuseTextureFS;
        static string DiffuseTextureFS2;

        static string SkyboxVS;
        static string SkyboxFS;
};

#endif // GAME_H
