#ifndef GFX_H
#define GFX_H

#include <string>
#include <SDL.h>

using namespace std;

class GFX
{
    public:
        GFX(string title, int width, int height, int bpc);
        virtual ~GFX();

        bool Init(); //Elk�k�sz�t minden grafik�val kapcsolatos dolgot
        void SwapBuffer(); //Megjelen�ti a k�pet

        int GetContextWidth();
        int GetContextHeight();

        void SizeChanged();
        void SetFullscreen(bool fs);
        void ToggleFullscreen();

    private:
        bool _fullscreen;

        int _bitsPerComponent;
        int _contextWidth;
        int _contextHeight;

        string _windowTitle;
        SDL_Window* _window;
        SDL_GLContext* _glContext;
        SDL_DisplayMode* _displayMode;

        int _currentDisplay;
};

#endif // GFX_H
