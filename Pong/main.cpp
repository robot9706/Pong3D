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

    cout << "Pong3D: Nagygy�rgy Bence 11.b ~ 2015" << endl << endl;

    //K�sz�tek egy pong p�ld�nyt
    Pong3D* pong = NULL;
    pong = new Pong3D();

    if(pong->Init()) //Ha minden rendben van (SDL, GLEW, OpenGL, stb.)
    {
        pong->Run(); //akkor mehet a menet :)

        //Takar�t�s...
        delete pong;
        pong = NULL;

        if(Pong3D::GlobalError){
            system("pause");
        }

        return 0;
    }

    //Takar�t�s...
    delete pong;
    pong = NULL;

    if(Pong3D::GlobalError){
        system("pause");
    }

    return 1;
}
