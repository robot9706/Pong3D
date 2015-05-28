#include <iostream>

#include <Pong3D.h>
#include <fstream>
#include <cstdlib>
#include <time.h>

using namespace std;

int main(int argc, char* args[])
{
    srand(time(NULL));
    setlocale(LC_ALL, "");

    cout << "Pong3D: Nagygyörgy Bence 11.b ~ 2015" << endl << endl;

    //Készítek egy pong példányt
    Pong3D* pong = NULL;
    pong = new Pong3D();

    if(pong->Init()) //Ha minden rendben van (SDL, GLEW, OpenGL, stb.)
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
