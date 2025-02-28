#include "Cuatroenlinea.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

void Cuatroenlinea::initWindow()
{
    this->videoMode.height = 700;
    this->videoMode.width = 700;
    this->window = new sf::RenderWindow(this->videoMode, "4 en linea", sf::Style::Titlebar | sf::Style::Close);
}

void Cuatroenlinea::initVariables()
{
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            this->tablero[i][j] = ' ';
        }
    }
    this->jugadorActual = 'O';
    this->modoJuego = 0; // Valor predeterminado
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

Cuatroenlinea::Cuatroenlinea()
{
    sf::RenderWindow menuWindow(sf::VideoMode(700, 700), "Menu 4 en Linea");
    this->modoJuego = this->mostrarMenu(menuWindow);
    
    if (this->modoJuego == -1) exit(0); // Si el usuario cierra el menú, termina el programa

    this->initWindow();
    this->initVariables();
    this->initTexto();
}

Cuatroenlinea::~Cuatroenlinea()
{
    delete this->window;
}

const bool Cuatroenlinea::getWindowIsOpen() const
{
    return this->window->isOpen();
}

void Cuatroenlinea::mostrarTurno()
{
    this->turnoTexto.setString("Turno del jugador: " + string(1, this->jugadorActual));
}

void Cuatroenlinea::actualizarEventos()
{
    while (this->window->pollEvent(this->ev))
    {
        if (this->ev.type == sf::Event::Closed)
        {
            this->window->close();
        }
    }
}

void Cuatroenlinea::actualizar()
{
    this->actualizarEventos();
}

void Cuatroenlinea::render()
{
    this->window->clear(sf::Color(132,182,244));
    this->window->draw(this->turnoTexto);
    this->window->display();
}

int Cuatroenlinea::mostrarMenu(sf::RenderWindow& window)
{
    if (!this->font.loadFromFile("resources/font.ttf")) {
        cerr << "Error al cargar la fuente" << endl;
        return -1;
    }
    
    // Crear los textos
    Text titulo, opcion1, opcion2;

    titulo.setFont(this->font);
    titulo.setString("4 en Linea");
    titulo.setCharacterSize(28); // Tamaño más pequeño
    titulo.setFillColor(Color::Black);

    opcion1.setFont(this->font);
    opcion1.setString("1. Jugar contra otra persona");
    opcion1.setCharacterSize(18); // Tamaño más pequeño
    opcion1.setFillColor(Color::Black);

    opcion2.setFont(this->font);
    opcion2.setString("2. Jugar contra la AI");
    opcion2.setCharacterSize(18); // Tamaño más pequeño
    opcion2.setFillColor(Color::Black);

    // Obtener el tamaño del texto para centrarlo correctamente
    FloatRect boundsTitulo = titulo.getLocalBounds();
    FloatRect boundsOpcion1 = opcion1.getLocalBounds();
    FloatRect boundsOpcion2 = opcion2.getLocalBounds();

    float centerX = window.getSize().x / 2.0f;

    titulo.setPosition(centerX - boundsTitulo.width / 2.0f, 50);  // Más arriba
    opcion1.setPosition(centerX - boundsOpcion1.width / 2.0f, 130); // Más abajo
    opcion2.setPosition(centerX - boundsOpcion2.width / 2.0f, 180); // Más abajo

    while (window.isOpen()) {
        sf::Event evento;
        while (window.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) {
                window.close();
                return -1;
            }
            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Keyboard::Num1) {
                    window.close();
                    return 0;
                }
                if (evento.key.code == sf::Keyboard::Num2) {
                    window.close();
                    return 1;
                }
            }
        }

        window.clear(sf::Color::White);
        window.draw(titulo);
        window.draw(opcion1);
        window.draw(opcion2);
        window.display();
    }
    return -1;
}