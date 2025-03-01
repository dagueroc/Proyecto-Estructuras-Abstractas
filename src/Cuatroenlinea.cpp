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
    this->mostrarMenu();  // Se elige el modo antes de iniciar el juego
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
    this->window->clear(sf::Color(132,182,244));

    this->window->draw(this->turnoTexto);

    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            sf::CircleShape cell(40.0f); 
            cell.setPosition(10 + j * (100.0f), (i + 1) * (100.0f));
            cell.setOutlineThickness(1.75f);
            cell.setOutlineColor(sf::Color(96, 109, 166));
            cell.setFillColor(sf::Color::White);
    
            this->window->draw(cell);
    
            if (tablero[i][j] != ' ') {
                sf::CircleShape ficha(40.0f);
                ficha.setFillColor(tablero[i][j] == 'O' ? sf::Color(255,105,97) : sf::Color(255,253,150));
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


void Cuatroenlinea::mostrarMenu()
{
    sf::Font menuFont;
    if (!menuFont.loadFromFile("resources/font.ttf"))
    {
        cerr << "Error al cargar la fuente" << endl;
        return;
    }

    sf::Text titulo("4 en Linea", menuFont, 20);
    titulo.setFillColor(sf::Color::Black);
    titulo.setPosition(250, 100);

    sf::RectangleShape caja1(sf::Vector2f(400, 50));
    caja1.setFillColor(sf::Color(105, 97, 255));
    caja1.setOutlineColor(sf::Color::Black);
    caja1.setOutlineThickness(2);
    caja1.setPosition(150, 240);

    sf::Text opcion1("Jugar contra otro jugador", menuFont, 15);
    opcion1.setFillColor(sf::Color(242, 240, 235));
    opcion1.setPosition(162, 255);

    sf::RectangleShape caja2(sf::Vector2f(400, 50));
    caja2.setFillColor(sf::Color(105, 97, 255));
    caja2.setOutlineColor(sf::Color::Black);
    caja2.setOutlineThickness(2);
    caja2.setPosition(150, 340);

    sf::Text opcion2("Jugar contra la IA", menuFont, 15);
    opcion2.setFillColor(sf::Color(242, 240, 235));
    opcion2.setPosition(210, 355);

    while (this->window->isOpen())
    {
        sf::Event event;
        while (this->window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                this->window->close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);

                    if (caja1.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                    {
                        this->contraIA = false;
                        return;
                    }
                    else if (caja2.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                    {
                        this->contraIA = true;

                        sf::RectangleShape cajaFacil(sf::Vector2f(400, 50));
                        cajaFacil.setFillColor(sf::Color::Transparent);
                        cajaFacil.setOutlineColor(sf::Color::Black);
                        cajaFacil.setOutlineThickness(2);
                        cajaFacil.setPosition(150, 240);

                        sf::Text opcionFacil("Modo Aspiradora", menuFont, 15);
                        opcionFacil.setFillColor(sf::Color::Black);
                        opcionFacil.setPosition(162, 255);

                        sf::RectangleShape cajaDificil(sf::Vector2f(400, 50));
                        cajaDificil.setFillColor(sf::Color::Transparent);
                        cajaDificil.setOutlineColor(sf::Color::Black);
                        cajaDificil.setOutlineThickness(2);
                        cajaDificil.setPosition(150, 340);

                        sf::Text opcionDificil("Modo Ultron", menuFont, 15);
                        opcionDificil.setFillColor(sf::Color::Black);
                        opcionDificil.setPosition(210, 355);

                        while (this->window->isOpen())
                        {
                            sf::Event subEvent;
                            while (this->window->pollEvent(subEvent))
                            {
                                if (subEvent.type == sf::Event::Closed)
                                    this->window->close();

                                if (subEvent.type == sf::Event::MouseButtonPressed)
                                {
                                    if (subEvent.mouseButton.button == sf::Mouse::Left)
                                    {
                                        sf::Vector2i subMousePos = sf::Mouse::getPosition(*this->window);

                                        if (cajaFacil.getGlobalBounds().contains(static_cast<sf::Vector2f>(subMousePos)))
                                        {
                                            this->dificultadIA = "Aspiradora";
                                            return;
                                        }
                                        else if (cajaDificil.getGlobalBounds().contains(static_cast<sf::Vector2f>(subMousePos)))
                                        {
                                            this->dificultadIA = "Ultron";
                                            return;
                                        }
                                    }
                                }
                            }

                            sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);

                            if (cajaFacil.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                            {
                                opcionFacil.setFillColor(sf::Color::Red);
                            }
                            else
                            {
                                opcionFacil.setFillColor(sf::Color::Black);
                            }

                            if (cajaDificil.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                            {
                                opcionDificil.setFillColor(sf::Color::Red);
                            }
                            else
                            {
                                opcionDificil.setFillColor(sf::Color::Black);
                            }

                            this->window->clear(sf::Color::White);
                            this->window->draw(titulo);
                            this->window->draw(cajaFacil);
                            this->window->draw(opcionFacil);
                            this->window->draw(cajaDificil);
                            this->window->draw(opcionDificil);
                            this->window->display();
                        }
                    }
                }
            }
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);

        if (caja1.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
        {
            opcion1.setFillColor(sf::Color::Red);
        }
        else
        {
            opcion1.setFillColor(sf::Color(242, 240, 235));
        }

        if (caja2.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
        {
            opcion2.setFillColor(sf::Color::Red);
        }
        else
        {
            opcion2.setFillColor(sf::Color(242, 240, 235));
        }

        this->window->clear(sf::Color::White);
        this->window->draw(titulo);
        this->window->draw(caja1);
        this->window->draw(opcion1);
        this->window->draw(caja2);
        this->window->draw(opcion2);
        this->window->display();
    }
}



void Cuatroenlinea::mostrarMenu()
{
    sf::Font menuFont;
    if (!menuFont.loadFromFile("resources/font.ttf"))
    {
        cerr << "Error al cargar la fuente" << endl;
        return;
    }

    sf::Text titulo("4 en Linea", menuFont, 20);
    titulo.setFillColor(sf::Color::Black);
    titulo.setPosition(250, 100);

    sf::RectangleShape caja1(sf::Vector2f(400, 50));
    caja1.setFillColor(sf::Color(105, 97, 255));
    caja1.setOutlineColor(sf::Color::Black);
    caja1.setOutlineThickness(2);
    caja1.setPosition(150, 240);

    sf::Text opcion1("Jugar contra otro jugador", menuFont, 15);
    opcion1.setFillColor(sf::Color(242, 240, 235));
    opcion1.setPosition(162, 255);

    sf::RectangleShape caja2(sf::Vector2f(400, 50));
    caja2.setFillColor(sf::Color(105, 97, 255));
    caja2.setOutlineColor(sf::Color::Black);
    caja2.setOutlineThickness(2);
    caja2.setPosition(150, 340);

    sf::Text opcion2("Jugar contra la IA", menuFont, 15);
    opcion2.setFillColor(sf::Color(242, 240, 235));
    opcion2.setPosition(210, 355);

    while (this->window->isOpen())
    {
        sf::Event event;
        while (this->window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                this->window->close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);

                    if (caja1.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                    {
                        this->contraIA = false;
                        return;
                    }
                    else if (caja2.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                    {
                        this->contraIA = true;

                        sf::RectangleShape cajaFacil(sf::Vector2f(400, 50));
                        cajaFacil.setFillColor(sf::Color::Transparent);
                        cajaFacil.setOutlineColor(sf::Color::Black);
                        cajaFacil.setOutlineThickness(2);
                        cajaFacil.setPosition(150, 240);

                        sf::Text opcionFacil("Modo Aspiradora", menuFont, 15);
                        opcionFacil.setFillColor(sf::Color::Black);
                        opcionFacil.setPosition(162, 255);

                        sf::RectangleShape cajaDificil(sf::Vector2f(400, 50));
                        cajaDificil.setFillColor(sf::Color::Transparent);
                        cajaDificil.setOutlineColor(sf::Color::Black);
                        cajaDificil.setOutlineThickness(2);
                        cajaDificil.setPosition(150, 340);

                        sf::Text opcionDificil("Modo Ultron", menuFont, 15);
                        opcionDificil.setFillColor(sf::Color::Black);
                        opcionDificil.setPosition(210, 355);

                        while (this->window->isOpen())
                        {
                            sf::Event subEvent;
                            while (this->window->pollEvent(subEvent))
                            {
                                if (subEvent.type == sf::Event::Closed)
                                    this->window->close();

                                if (subEvent.type == sf::Event::MouseButtonPressed)
                                {
                                    if (subEvent.mouseButton.button == sf::Mouse::Left)
                                    {
                                        sf::Vector2i subMousePos = sf::Mouse::getPosition(*this->window);

                                        if (cajaFacil.getGlobalBounds().contains(static_cast<sf::Vector2f>(subMousePos)))
                                        {
                                            this->dificultadIA = "Aspiradora";
                                            return;
                                        }
                                        else if (cajaDificil.getGlobalBounds().contains(static_cast<sf::Vector2f>(subMousePos)))
                                        {
                                            this->dificultadIA = "Ultron";
                                            return;
                                        }
                                    }
                                }
                            }

                            sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);

                            if (cajaFacil.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                            {
                                opcionFacil.setFillColor(sf::Color::Red);
                            }
                            else
                            {
                                opcionFacil.setFillColor(sf::Color::Black);
                            }

                            if (cajaDificil.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                            {
                                opcionDificil.setFillColor(sf::Color::Red);
                            }
                            else
                            {
                                opcionDificil.setFillColor(sf::Color::Black);
                            }

                            this->window->clear(sf::Color::White);
                            this->window->draw(titulo);
                            this->window->draw(cajaFacil);
                            this->window->draw(opcionFacil);
                            this->window->draw(cajaDificil);
                            this->window->draw(opcionDificil);
                            this->window->display();
                        }
                    }
                }
            }
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);

        if (caja1.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
        {
            opcion1.setFillColor(sf::Color::Red);
        }
        else
        {
            opcion1.setFillColor(sf::Color(242, 240, 235));
        }

        if (caja2.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
        {
            opcion2.setFillColor(sf::Color::Red);
        }
        else
        {
            opcion2.setFillColor(sf::Color(242, 240, 235));
        }

        this->window->clear(sf::Color::White);
        this->window->draw(titulo);
        this->window->draw(caja1);
        this->window->draw(opcion1);
        this->window->draw(caja2);
        this->window->draw(opcion2);
        this->window->display();
    }
}