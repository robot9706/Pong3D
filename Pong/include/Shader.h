#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/mat4x4.hpp>

using namespace std;

class Shader
{
    public:
        Shader();

        bool Load(string vsCode, string fsCode); //Bet�lti a shader-t
        void Apply(); //Aktiv�lja

        void BindLocation(string name, int attrib); //Be�ll�tja, hogy a shader-ben egy bemenet ("name"), melyik vertex attrib�tumnak felel meg

        unsigned int GetUniform(string name); //Visszaadja egy shader v�ltoz� hely�t

        //Shader v�ltoz� �rt�k megad�sa:
        void SetMatrix(unsigned int id, float* mat);
        void SetMatrix(unsigned int id, glm::mat4 mat);
        void SetVector4(unsigned int id, float x, float y, float z, float w);
        void SetVector3(unsigned int id, float x, float y, float z);
        void SetSampler(unsigned int id, int sampler, unsigned int texture);

        virtual ~Shader();
    protected:
    private:
        unsigned int _program;
};

#endif // SHADER_H
