#include <iostream>

#include <Pong3D.h>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <SoundManager.h>

using namespace std;

int main(int argc, char* args[])
{
    int windowWidth = 1280;
    int windowHeight = 720;
    bool fullscreen = false;

    if(argc > 0)
    {
        for(int x = 0;x<argc;x++)
        {
            string val = args[x];

            if(val == "nosound")
            {
                SoundManager::Disable();
            }
            else if(val == "fullscreen")
            {
                fullscreen = true;
            }
            else if(val.length() > 2)
            {
                if(val.substr(0,2) == "w:")
                {
                    windowWidth = atoi(val.substr(2, val.length() - 2).c_str());
                }
                else if(val.substr(0,2) == "h:")
                {
                    windowHeight = atoi(val.substr(2, val.length() - 2).c_str());
                }
            }
        }
    }

    srand(time(NULL));
    setlocale(LC_ALL, "");

    cout << "Pong3D: Nagygyörgy Bence 11.b ~ 2015" << endl << endl;

    //Készítek egy pong példányt
    Pong3D* pong = NULL;
    pong = new Pong3D();

    if(pong->Init(windowWidth, windowHeight, fullscreen)) //Ha minden rendben van (SDL, GLEW, OpenGL, stb.)
    {
        pong->Run(); //akkor mehet a menet :)

        //Takarítás...
        delete pong;
        pong = NULL;

        if(Pong3D::GlobalError){
            system("pause");
        }

        return 0;
    }

    //Takarítás...
    delete pong;
    pong = NULL;

    if(Pong3D::GlobalError){
        system("pause");
    }

    return 1;
}
