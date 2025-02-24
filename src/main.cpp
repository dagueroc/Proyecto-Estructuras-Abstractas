
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>


#include <iostream>
#include <optional>
#include "Cuatroenlinea.h"


using namespace std;
using namespace sf;

    

int main()
{
    //Cuatroenlinea cuatroenlinea;
    cout << "Bienvenido a Cuatro en Linea" << endl;
    
    while (cuatroenlinea.getWindowIsOpen())
    {
        //Update
        cuatroenlinea.actualizar();
        //Render
        cuatroenlinea.render();
        
    }
}