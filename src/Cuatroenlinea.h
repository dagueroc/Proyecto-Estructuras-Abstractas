#ifndef CUATROENLINEA_H
#define CUATROENLINEA_H

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

class Cuatroenlinea {
    public:
        // Constructores y destructores
        Cuatroenlinea();
        virtual ~Cuatroenlinea();

        // Accesos
        const bool getWindowIsOpen() const;

        // Métodos principales
        void actualizarEventos();
        void actualizar();
        void render();
        bool colocarFicha(int columna);
        bool verificarVictoria(char jugador);
        void cambiarTurno();
        void reiniciarJuego();
        void mostrarTurno();

        // Menú de selección de modo de juego
        int mostrarMenu(sf::RenderWindow& window);

    private:
        // Ventana principal del juego
        RenderWindow* window;
        Event ev;
        VideoMode videoMode;

        // Métodos de inicialización
        void initWindow();
        void initVariables();
        void initTexto();

        // Variables del tablero y lógica del juego
        static const int FILAS = 6;
        static const int COLUMNAS = 7;
        char tablero[FILAS][COLUMNAS];
        char jugadorActual;
        int modoJuego; // 0: Jugador vs Jugador, 1: Jugador vs AI

        // Fuente y textos para mostrar la interfaz
        Font font;
        Text turnoTexto;
};

#endif // CUATROENLINEA_H