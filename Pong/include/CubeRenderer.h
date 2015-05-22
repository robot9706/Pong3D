#ifndef CUBERENDERER_H
#define CUBERENDERER_H


class CubeRenderer
{
    public:
        static void Init();
        static void Cleanup();
        static void BindVAO();
        static void UnbindVAO();

    private:
        static unsigned int _vbo;
        static unsigned int _ibo;
        static unsigned int _vao;

        static int _stride;
};

#endif // CUBERENDERER_H
