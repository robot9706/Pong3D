//============================
//Nagygyörgy Bence 11.b - 2015
//============================
#include "Shader.h"

#include <string>
#include <glew.h>
#include <vector>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

using namespace std;

Shader::Shader()
{
}

bool Shader::Load(string vsCode, string fsCode)
{
    int result = GL_FALSE;
    int infoLength = 0;

    //Vertex shader
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    {
        const char* vsPointer = vsCode.c_str();

        glShaderSource(vs, 1, &vsPointer, NULL);
        glCompileShader(vs);

        //Hiba ellenõrzés
        glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &infoLength);
        if (infoLength > 0 && result == GL_FALSE )
        {
            vector<char> error(infoLength+1);
            glGetShaderInfoLog(vs, infoLength, NULL, &error[0]);

            cout << "VS hiba: " << &error[0] << endl;

            glDeleteShader(vs);

            return false;
        }
    }

    //Fragment shader
    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    {
        const char* fsPointer = fsCode.c_str();

        glShaderSource(fs, 1, &fsPointer, NULL);
        glCompileShader(fs);

        //Hiba ellenõrzés
        glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
        glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &infoLength);
        if (infoLength > 0 && result == GL_FALSE )
        {
            vector<char> error(infoLength+1);
            glGetShaderInfoLog(fs, infoLength, NULL, &error[0]);

            cout << "FS hiba: " << &error[0] << endl;

            glDeleteShader(fs);

            return false;
        }
    }

    //Shader program
    _program = glCreateProgram();
    glAttachShader(_program, vs);
    glAttachShader(_program, fs);
    glLinkProgram(_program);

    //Ellenõrzés
	glGetProgramiv(_program, GL_LINK_STATUS, &result);
	glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLength);
	if (infoLength > 0 && result == GL_FALSE )
    {
		vector<char> error(infoLength+1);
		glGetProgramInfoLog(_program, infoLength, NULL, &error[0]);

		cout << "Shader hiba: " << error[0] << endl;
	}

    //Takarítás
    glDeleteShader(vs);
    glDeleteShader(fs);

    return true;
}

void Shader::Apply()
{
    glUseProgram(_program);
}

unsigned int Shader::GetUniform(string name)
{
    return glGetUniformLocation(_program, name.c_str());
}

void Shader::SetMatrix(unsigned int id, float* mat)
{
    glUniformMatrix4fv(id, 1, false, mat);
}

void Shader::SetMatrix(unsigned int id, glm::mat4 mat)
{
    glUniformMatrix4fv(id, 1, false, glm::value_ptr(mat));
}

void Shader::SetVector4(unsigned int id, float x, float y, float z, float w)
{
    glUniform4f(id, x, y, z, w);
}

void Shader::SetVector3(unsigned int id, float x, float y, float z)
{
    glUniform3f(id, x, y, z);
}

void Shader::SetSampler(unsigned int id, int sampler, unsigned int tex)
{
    glActiveTexture(GL_TEXTURE0 + sampler);
    glBindTexture(GL_TEXTURE_2D, tex);

    glUniform1f(id, sampler);
}

void Shader::BindLocation(string name, int attrib)
{
    glBindAttribLocation(_program, attrib, name.c_str());
}

Shader::~Shader()
{
    glDeleteProgram(_program);
}
