//============================
//Nagygyörgy Bence 11.b - 2015
//============================
#define GLM_FORCE_RADIAN

#include "SpriteBatch.h"

#include <glew.h>
#include <Pong3D.h>
#include <Matrix4.h>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* Vertex: XYZST = 5 float -> 20 byte / vertex
 * 0-2: Hely (Z = 0)
 * 3-4: UV
 */

SpriteBatch* SpriteBatch::Singelton = NULL;

int SpriteBatch::Stride = sizeof(float) * 5;
int SpriteBatch::TextureOffset = sizeof(float) * 3;

/*
0-{0,0,0} |----| 1-{1,0,0}
          |\   |
          | \  |
          |  \ |
          |   \|
3-{0,1,0} |----| 2-{1,1,0}
*/

float SpriteBatch::VertexData[] = { 0, 0, 0, 0, 0,
                                    1, 0, 0, 1, 0,
                                    1, 1, 0, 1, 1,
                                    0, 1, 0, 0, 1 };
short SpriteBatch::IndexData[] = { 1, 0, 2, 3, 2, 0 };

string SpriteBatch::VertexShaderCode =
"#version 150\n"
"uniform mat4 MVP;"
"in vec3 in_position;"
"in vec2 in_tex;"
"smooth out vec2 texUV;"
"void main()"
"{"
"	texUV = in_tex;"
"   gl_Position = MVP * vec4(in_position, 1);"
"}";

string SpriteBatch::FragmentShaderCode =
"#version 150\n"
"out vec4 out_frag_color;"
"uniform sampler2D Texture;"
"uniform vec4 MulColor;"
"smooth in vec2 texUV;"
"void main()"
"{"
"	out_frag_color = texture2D(Texture, texUV) * MulColor;"
//"	if(out_frag_color.a<0.1)"
//"		discard;"
"}";

Shader* SpriteBatch::SpriteShader = NULL;

SpriteBatch::SpriteBatch(Pong3D* pong)
{
    _begun = false;
    _pong = pong;

    //Készítsük el a shader-t
    if(SpriteShader == NULL)
    {
        SpriteShader = new Shader();
        if(!SpriteShader->Load(VertexShaderCode, FragmentShaderCode))
        {
            Pong3D::GlobalError = true;
        }

        SpriteShader->Apply();
        SpriteShader->BindLocation("in_position", 0);
        SpriteShader->BindLocation("in_tex", 1);
        _mvpLocation = SpriteShader->GetUniform("MVP");
        _mulColorLocation = SpriteShader->GetUniform("MulColor");
        _textureLocation = SpriteShader->GetUniform("Texture");
        SpriteShader->SetVector4(_mulColorLocation, 1, 1, 1, 1);
    }

    //Készítsük el a grafikus dolgokat (VBO, IBO, VAO)
    {
        //Kell 2db buffer
        glGenBuffers(1, &_vbo);
        glGenBuffers(1, &_ibo);

        //És 1 vao (vertex array object)
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        //VBO
        {
            //Feltöltöm a buffert adatokkal
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (sizeof(VertexData)/sizeof(*VertexData)), VertexData, GL_STATIC_DRAW);

            //Vertex beállítások:
            glEnableVertexAttribArray(0);
            //Elsõ 3db float az XYZ azaz a 0. attribútum
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Stride, (void*)0); //void* c++ pls

            glEnableVertexAttribArray(1);
            //Az utolsó float az I azaz az 1. attribútum
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Stride, (void*)TextureOffset);
        }

        //IBO
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short) * (sizeof(IndexData)/sizeof(*IndexData)), IndexData, GL_STATIC_DRAW); //Az IBO-t is feltöltöm adattal
        }

        //Kész
        glBindVertexArray(0); //(A VAO megtanult minden beállítást)

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void SpriteBatch::Begin()
{
    if(_begun)
        return;

    _begun = true;

    glDisable(GL_DEPTH_TEST);

    _projMatrix = glm::ortho(0.0f, (float)_pong->GetGFX()->GetContextWidth(), (float)_pong->GetGFX()->GetContextHeight(), 0.0f, 0.0f, 1.0f);

    glBindVertexArray(_vao);

    SpriteShader->Apply();
}

void SpriteBatch::DrawTexture(Texture2D* tex, float destX, float destY, float destW, float destH, float originX, float originY, float rotation, float scale)
{
    //Minden méretet HD fölbontásra adok meg és itt alakítom ezeket az aktuális fölbontásra
    destX = (destX / 1280) * _pong->GetGFX()->GetContextWidth();
    destY = (destY / 720) * _pong->GetGFX()->GetContextHeight();
    destW = (destW / 1280) * _pong->GetGFX()->GetContextWidth();
    destH = (destH / 720) * _pong->GetGFX()->GetContextHeight();

    originX = (originX / 1280) * _pong->GetGFX()->GetContextWidth();
    originY = (originY / 720) * _pong->GetGFX()->GetContextHeight();

    glm::mat4 w = CalculateWorld(destX, destY, destW, destH, originX, originY, rotation, scale);
    glm::mat4 mvp = _projMatrix * w;

    SpriteShader->SetMatrix(_mvpLocation, glm::value_ptr(mvp));
    SpriteShader->SetSampler(_textureLocation, 0, tex->GetID());

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
}

void SpriteBatch::SetColor(float r, float g, float b, float a)
{
    if(!_begun)
        return;

    SpriteShader->SetVector4(_mulColorLocation, r, g, b, a);
}

void SpriteBatch::SetColor(float c, float a)
{
    if(!_begun)
        return;

    SpriteShader->SetVector4(_mulColorLocation, c, c, c, a);
}

glm::mat4 SpriteBatch::CalculateWorld(float destX, float destY, float destW, float destH, float originX, float originY, float rotation, float scale)
{
    glm::mat4 sc = glm::scale(glm::mat4(1.0f), glm::vec3((float)(destW), (float)(destH), 1));

    glm::mat4 rot = glm::translate(glm::mat4(1.0f), glm::vec3(originX, originY, 0));
    rot = glm::rotate(rot, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    rot = glm::scale(rot, glm::vec3(scale, scale, scale));
    rot = glm::translate(rot, glm::vec3(-originX, -originY, 0));

    glm::mat4 w = glm::translate(glm::mat4(1.0f), glm::vec3(destX, destY, 0));

    return w * rot * sc;
}

void SpriteBatch::End()
{
    glBindVertexArray(0);

    _begun = false;

    glEnable(GL_DEPTH_TEST);
}

SpriteBatch::~SpriteBatch()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ibo);

    delete SpriteShader;
    SpriteShader = NULL;
}
