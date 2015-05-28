#ifndef CUBERENDERER_H
#define CUBERENDERER_H

//Oszt�ly, amely dobozokat tud rajzolni
class CubeRenderer
{
    public:
        static void Init();
        static void Cleanup();
        static void BindVAO();
        static void UnbindVAO();

    private:
        //OpenGL cuccos
        static unsigned int _vbo;
        static unsigned int _ibo;
        static unsigned int _vao;

        static int _stride; //Ez t�rolja, hogy egy vertex h�ny byte
};

#endif // CUBERENDERER_H
