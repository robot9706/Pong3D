#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H

#include <string>
#include <Shader.h>
#include <glm/mat4x4.hpp>
#include <Texture2D.h>

using namespace std;

class Pong3D;
class Texture2D;

//2D rajzoló
class SpriteBatch
{
    public:
        static SpriteBatch* Instance; //SpriteBatch példány

        SpriteBatch(Pong3D* pong);
        virtual ~SpriteBatch();

        void Begin();
        void End();

        void DrawTexture(Texture2D* tex, float destX, float destY, float destW, float destH, float originX, float originY, float rotation, float scale);
        void SetColor(float r, float g, float b, float a);
        void SetColor(float c, float a);

        glm::mat4 CalculateWorld(float destX, float destY, float destW, float destH, float originX, float originY, float rotation, float scale);
    private:
        unsigned int _vbo;
        unsigned int _ibo;
        unsigned int _vao;

        unsigned int _mvpLocation;
        unsigned int _mulColorLocation;
        unsigned int _textureLocation;
        unsigned int _texCoordsLocation;

        glm::mat4 _projMatrix;

        bool _begun;

        Pong3D* _pong;

        static int Stride;
        static int TextureOffset;
        static float VertexData[];
        static short IndexData[];
        static string VertexShaderCode;
        static string FragmentShaderCode;
        static Shader* SpriteShader;
};

#endif // SPRITEBATCH_H
