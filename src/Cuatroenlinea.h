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
        bool colocarFicha(int columna);
        bool verificarVictoria(char jugador);
        void cambiarTurno();
        void reiniciarJuego();
        void mostrarTurno();
        void mostrarMenu();

    private:
        // Variables
        RenderWindow* window;
        Event ev;
        VideoMode videoMode;
        void initWindow();
        void initVariables();

        // Tablero y lógica del juego
        static const int FILAS = 6;
        static const int COLUMNAS = 7;
        char tablero[FILAS][COLUMNAS];
        char jugadorActual;
        bool contraIA;
        string dificultadIA;

        // Fuente y texto para mostrar el turno
        Font font;
        Text turnoTexto;
        void initTexto();
};

#endif // CUATROENLINEA_H
