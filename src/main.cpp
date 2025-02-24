#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // Crear ventana principal
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

    // Bucle principal de la ventana
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) { // Cambiar para usar la firma correcta
            if (event.type == sf::Event::Closed) { // Verificar el tipo de evento
                window.close();
            }
        }

        // Limpiar la ventana con un color negro
        window.clear(sf::Color::Black);

        // Mostrar los cambios en la ventana
        window.display();
    }

    return 0;
}