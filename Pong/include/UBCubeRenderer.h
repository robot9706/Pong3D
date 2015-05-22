#ifndef UBCUBERENDERER_H
#define UBCUBERENDERER_H


class UBCubeRenderer
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

#endif // UBCUBERENDERER_H
