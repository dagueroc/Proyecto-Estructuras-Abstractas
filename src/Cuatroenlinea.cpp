#include "Cuatroenlinea.h"

using namespace std;

// Métodos privados

void Cuatroenlinea::initWindow()
{
    this->videoMode.height = 600;
    this->videoMode.width = 800;

    this->window = new sf::RenderWindow(
        this->videoMode, 
        "4 en linea", 
        sf::Style::Titlebar | sf::Style::Close
    );
}

void Cuatroenlinea::initVariables()
{
    //this->window = nullptr;
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
        
        case sf::Event::KeyPressed:
            if (this->ev.key.code == sf::Keyboard::Key::Escape)
            {
                this->window->close();
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
    this->window->clear(sf::Color::White);

    // Aquí se dibuja todo

    this->window->display();
}