#ifndef CUATROENLINEA_H
#define CUATROENLINEA_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>

using namespace std;
using namespace sf;
//Funcionalidades del juego


class Cuatroenlinea {

    public:
        // Constructores y destructores
        Cuatroenlinea();
        virtual ~Cuatroenlinea();

        // Accesos
        
        const bool getWindowIsOpen() const;


        // Metodos
        void actualizarEventos();
        void actualizar();
        void render();

    private:

        //Variables
        RenderWindow* window;
        Event ev;

        VideoMode videoMode;
        void initWindow();
        void initVariables();


};

#endif // CUATROENLINEA_H