#ifndef SPHERERENDERER_H
#define SPHERERENDERER_H


class SphereRenderer
{
    public:
        static void Init();
        static void Cleanup();
        static void BindVAO();
        static void UnbindVAO();

        static int IndexLength;

    private:
        static unsigned int _vbo;
        static unsigned int _ibo;
        static unsigned int _vao;

        static int _stride;
};

#endif // SPHERERENDERER_H
