#ifndef UVPLANE_H
#define UVPLANE_H


class UVPlane
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

#endif // UVPLANE_H
