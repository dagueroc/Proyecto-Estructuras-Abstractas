#include "Cuatroenlinea.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

// Métodos privados
void Cuatroenlinea::initWindow()
{
    this->videoMode.height = 700;
    this->videoMode.width = 700;

    this->window = new sf::RenderWindow(
        this->videoMode, 
        "4 en linea", 
        sf::Style::Titlebar | sf::Style::Close
    );
}

void Cuatroenlinea::initVariables()
{
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            this->tablero[i][j] = ' ';
        }
    }
    this->jugadorActual = 'O';
}

void Cuatroenlinea::initTexto()
{
    if (!this->font.loadFromFile("resources/font.ttf")) {
        cerr << "Error al cargar la fuente" << endl;
    }
    this->turnoTexto.setFont(this->font);
    this->turnoTexto.setCharacterSize(24);
    this->turnoTexto.setFillColor(sf::Color::Black);
    this->turnoTexto.setPosition(10.f, 10.f);
    this->mostrarTurno();
}

// Accesos
const bool Cuatroenlinea::getWindowIsOpen() const
{
    return this->window->isOpen();
}

// Constructores y destructores
Cuatroenlinea::Cuatroenlinea()
{
    this->initWindow();
    this->initVariables();
    this->initTexto();
}

Cuatroenlinea::~Cuatroenlinea()
{
    delete this->window;
}

// Métodos
void Cuatroenlinea::actualizarEventos()
{
    while (this->window->pollEvent(this->ev))
    {
        switch (this->ev.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::MouseButtonPressed:
            if (this->ev.mouseButton.button == sf::Mouse::Left)
            {
                int columna = this->ev.mouseButton.x / 100;
                if (columna >= 0 && columna < COLUMNAS)
                {
                    if (this->colocarFicha(columna)) {
                        if (this->verificarVictoria(this->jugadorActual)) {
                            cout << "¡El jugador " << this->jugadorActual << " ganó!" << endl;
                            this->reiniciarJuego();
                        } else {
                            this->cambiarTurno();
                        }
                    }
                }
            }
            break;
        }
    }
}

void Cuatroenlinea::actualizar()
{
    this->actualizarEventos();
}

void Cuatroenlinea::render()
{
    this->window->clear(sf::Color::Blue);

    this->window->draw(this->turnoTexto);

    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            sf::CircleShape cell(40.0f); 
            cell.setPosition(10 + j * (100.0f), (i + 1) * (100.0f));
            cell.setOutlineThickness(2.0f);
            cell.setOutlineColor(sf::Color::Black);
            cell.setFillColor(sf::Color::White);
    
            this->window->draw(cell);
    
            if (tablero[i][j] != ' ') {
                sf::CircleShape ficha(40.0f);
                ficha.setFillColor(tablero[i][j] == 'O' ? sf::Color::Red : sf::Color::Yellow);
                ficha.setPosition(10 + j * (100.0f), (i + 1) * (100.0f));
                this->window->draw(ficha);
            }
        }
    
    }
    this->window->display();
}

bool Cuatroenlinea::colocarFicha(int columna)
{
    for (int i = FILAS - 1; i >= 0; i--) {
        if (this->tablero[i][columna] == ' ') {
            this->tablero[i][columna] = this->jugadorActual;
            return true;
        }
    }
    return false;
}

void Cuatroenlinea::cambiarTurno()
{
    this->jugadorActual = (this->jugadorActual == 'O') ? 'X' : 'O';
    this->mostrarTurno();
}

void Cuatroenlinea::mostrarTurno()
{
    this->turnoTexto.setString("Turno del jugador: " + string(1, this->jugadorActual));
}

void Cuatroenlinea::reiniciarJuego()
{
    this->initVariables();
    this->mostrarTurno();
}

bool Cuatroenlinea::verificarVictoria(char jugador)
{
    
    return false;
}
