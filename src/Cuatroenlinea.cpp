#include "Cuatroenlinea.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <functional>

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
void Cuatroenlinea::initSonido()
{
    if (!this->buffer.loadFromFile("resources/boton.wav")) {
        cerr << "Error al cargar el sonido" << endl;
    } else {
        cout << "Sonido cargado correctamente" << endl;  
    }

    this->sonidoBoton.setBuffer(this->buffer);
    sonidoBoton.setVolume(75);

    // Cargar la música de fondo
    if (!this->musicaFondo.openFromFile("resources/menu.wav")) {
        cerr << "Error al cargar la música de fondo" << endl;
    } else {
        cout << "Música de fondo cargada correctamente" << endl;
    }
    musicaFondo.setVolume(50);

    if (!this->bufferBip.loadFromFile("resources/bip.wav")) {
        cerr << "Error al cargar el sonido" << endl;
    } else {
        cout << "Sonido cargado correctamente" << endl;  
    }
    this->sonidoBip.setBuffer(this->bufferBip);
    sonidoBip.setVolume(100);
    
    if (!this->bufferInicio.loadFromFile("resources/inicio.wav")) {
        cerr << "Error al cargar el sonido" << endl;
    } else {
        cout << "Sonido cargado correctamente" << endl;  
    }
    this->sonidoInicio.setBuffer(this->bufferInicio);
    sonidoInicio.setVolume(100);

    if (!this->musicaDerrota.openFromFile("resources/derrota.wav")) {
        cerr << "Error al cargar la música de derrota" << endl;
    } else {
        cout << "Música de derrota cargada correctamente" << endl;
    }
    musicaDerrota.setVolume(50);

    if (!this->musicaVictoria.openFromFile("resources/victoria.wav")) {
        cerr << "Error al cargar la música de victoria" << endl;
    } else {
        cout << "Música de victoria cargada correctamente" << endl;
    }
    musicaVictoria.setVolume(50);

    
}

void Cuatroenlinea::fadeOutMusica(sf::Music& musica, float duration) {
    float volume = musica.getVolume();
    float decrement = volume / (duration * 1000 / 30); // Decremento por cada 10 ms

    while (volume > 0) {
        volume -= decrement;
        if (volume < 0) volume = 0;
        musica.setVolume(volume);
        sf::sleep(sf::milliseconds(10));
    }
    musica.stop();
}

// Accesos
const bool Cuatroenlinea::getWindowIsOpen() const
{
    return this->window->isOpen();
}

// Constructores y destructores
Cuatroenlinea::Cuatroenlinea()
{
    this->initSonido();
    this->initWindow();
    this->mostrarMenu();  // Se elige el modo antes de iniciar el juego
    this->initVariables();
    this->initTexto();

    if (this->contraIA) {
        srand(time(0));  // Inicializa la semilla para la IA
    }
}

Cuatroenlinea::~Cuatroenlinea()
{
    delete this->window;
}

// Métodos
void Cuatroenlinea::actualizarEventos()
{
    bool sonidoBotonReproducido = false;
    while (this->window->pollEvent(this->ev))
    {
        switch (this->ev.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;

        case sf::Event::MouseButtonPressed:            // Si Ultron gana::Mouse::Left)
            {
                int columna = this->ev.mouseButton.x / 100; // Determina la columna de la celda
                if (columna >= 0 && columna < COLUMNAS)
                {
                    if (this->colocarFicha(columna)) {
                        vector<pair<int, int>> fichasGanadoras;
                        if (this->verificarVictoria(this->jugadorActual, fichasGanadoras)) {
                            this->parpadearFichasGanadoras(fichasGanadoras);
                            string nombreJugador = (this->jugadorActual == 'O') ? "Rojo" : "Amarillo";
                            cout << "¡El jugador " << nombreJugador << " es el ganador!" << endl;
                            this->musicaVictoria.setLoop(true);
                            this->musicaVictoria.play();
                            this->mostrarVentanaVictoria("Victoria de  " + nombreJugador);
                            this->reiniciarJuego();
                        } else {
                            this->cambiarTurno();
                            if (this->contraIA && this->jugadorActual == 'X') {
                                if (this->dificultadIA == "Aspiradora") {
                                    this->jugarContraAspi();
                                } else if (this->dificultadIA == "Ultron") {
                                    this->jugarContraUltron();
                                }
                            }
                        }
                    }
                }
            }
            break;

        case sf::Event::MouseMoved:
            if (this->enMenu)  
            {

                int columna = this->ev.mouseMove.x / 100;
                if (columna >= 0 && columna < COLUMNAS) {

                    if (!sonidoBotonReproducido) {

                        sonidoBotonReproducido = true;  
                    }
                }
                else {
                    sonidoBotonReproducido = false;  
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
            this->render(); 
            sf::sleep(sf::seconds(0.5));
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
    string nombreJugador = (this->jugadorActual == 'O') ? "Rojo" : "Amarillo";
    this->turnoTexto.setString("Turno del jugador: " + nombreJugador);
}

void Cuatroenlinea::reiniciarJuego()
{
    this->initVariables();
    this->mostrarTurno();
    musicaFondo.setVolume(50);
    musicaFondo.play();
}

bool Cuatroenlinea::verificarVictoria(char jugador, vector<pair<int, int>>& fichasGanadoras) {
    // Verificar filas
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS - 3; j++) {
            if (tablero[i][j] == jugador && tablero[i][j + 1] == jugador &&
                tablero[i][j + 2] == jugador && tablero[i][j + 3] == jugador) {
                fichasGanadoras = {{i, j}, {i, j + 1}, {i, j + 2}, {i, j + 3}};
                return true;
            }
        }
    }

    // Verificar columnas
    for (int j = 0; j < COLUMNAS; j++) {
        for (int i = 0; i < FILAS - 3; i++) {
            if (tablero[i][j] == jugador && tablero[i + 1][j] == jugador &&
                tablero[i + 2][j] == jugador && tablero[i + 3][j] == jugador) {
                fichasGanadoras = {{i, j}, {i + 1, j}, {i + 2, j}, {i + 3, j}};
                return true;
            }
        }
    }

    // Verificar diagonales (de izquierda a derecha)
    for (int i = 0; i < FILAS - 3; i++) {
        for (int j = 0; j < COLUMNAS - 3; j++) {
            if (tablero[i][j] == jugador && tablero[i + 1][j + 1] == jugador &&
                tablero[i + 2][j + 2] == jugador && tablero[i + 3][j + 3] == jugador) {
                fichasGanadoras = {{i, j}, {i + 1, j + 1}, {i + 2, j + 2}, {i + 3, j + 3}};
                return true;
            }
        }
    }

    // Verificar diagonales (de derecha a izquierda)
    for (int i = 0; i < FILAS - 3; i++) {
        for (int j = 3; j < COLUMNAS; j++) {
            if (tablero[i][j] == jugador && tablero[i + 1][j - 1] == jugador &&
                tablero[i + 2][j - 2] == jugador && tablero[i + 3][j - 3] == jugador) {
                fichasGanadoras = {{i, j}, {i + 1, j - 1}, {i + 2, j - 2}, {i + 3, j - 3}};
                return true;
            }
        }
    }

    return false;
}

void Cuatroenlinea::jugarContraAspi()
{
    int columna;
    do {
        columna = rand() % COLUMNAS;
    } while (this->tablero[0][columna] != ' ');

    this->colocarFicha(columna);
    vector<pair<int, int>> fichasGanadoras;
    if (this->verificarVictoria(this->jugadorActual, fichasGanadoras)) {
        this->parpadearFichasGanadoras(fichasGanadoras);
        if (this->jugadorActual == 'X') {
            cout << "¡Aspiradora es el ganador!" << endl;
            this->mostrarVentanaVictoria("Victoria de Aspiradora");
            this->musicaDerrota.setLoop(true);
            this->musicaDerrota.play();
        } else {
            string nombreJugador = "Rojo";
            cout << "¡El jugador " << nombreJugador << " es el ganador!" << endl;
            this->musicaVictoria.setLoop(true);
            this->musicaVictoria.play();
            this->mostrarVentanaVictoria("Victoria de " + nombreJugador);
        }
        this->reiniciarJuego();
    } else {
        this->cambiarTurno();
    }
}

void Cuatroenlinea::mostrarVentanaVictoria(const std::string &mensaje) {
    // Configurar el mensaje de victoria
    sf::Text textVictory;
    textVictory.setFont(this->font);
    textVictory.setString(mensaje);
    textVictory.setCharacterSize(20);
    textVictory.setFillColor(sf::Color::Black);
    sf::FloatRect textRect = textVictory.getLocalBounds();
    textVictory.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    textVictory.setPosition(this->window->getSize().x / 2.0f, 50);
    
    // Botón "Jugar de nuevo"
    sf::RectangleShape buttonPlay(sf::Vector2f(300, 50));
    buttonPlay.setFillColor(sf::Color::Green);
    buttonPlay.setPosition(200, 240);
    sf::Text textPlay;
    textPlay.setFont(this->font);
    textPlay.setString("Jugar de nuevo");
    textPlay.setCharacterSize(20);
    textPlay.setFillColor(sf::Color::Black);
    sf::FloatRect playTextRect = textPlay.getLocalBounds();
    textPlay.setOrigin(playTextRect.left + playTextRect.width / 2.0f, playTextRect.top + playTextRect.height / 2.0f);
    textPlay.setPosition(buttonPlay.getPosition().x + buttonPlay.getSize().x / 2.0f,
                         buttonPlay.getPosition().y + buttonPlay.getSize().y / 2.0f);
    
    // Botón "Salir"
    sf::RectangleShape buttonExit(sf::Vector2f(300, 50));
    buttonExit.setFillColor(sf::Color::Red);
    buttonExit.setPosition(200, 320);
    sf::Text textExit;
    textExit.setFont(this->font);
    textExit.setString("Salir");
    textExit.setCharacterSize(20);
    textExit.setFillColor(sf::Color::Black);
    sf::FloatRect exitTextRect = textExit.getLocalBounds();
    textExit.setOrigin(exitTextRect.left + exitTextRect.width / 2.0f, exitTextRect.top + exitTextRect.height / 2.0f);
    textExit.setPosition(buttonExit.getPosition().x + buttonExit.getSize().x / 2.0f,
                         buttonExit.getPosition().y + buttonExit.getSize().y / 2.0f);
    
    int opcionSeleccionada = 0; // 1: Jugar de nuevo, 2: Salir
    
    // Bucle de la ventana de victoria (este diálogo se ejecuta hasta que se elige una opción)
    while (this->window->isOpen() && opcionSeleccionada == 0) {
        sf::Event event;
        while (this->window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                this->window->close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                if (buttonPlay.getGlobalBounds().contains(mousePos)) {
                    opcionSeleccionada = 1;
                }
                else if (buttonExit.getGlobalBounds().contains(mousePos)) {
                    opcionSeleccionada = 2;
                }
            }
        }
        
        this->window->clear(sf::Color::White);
        this->window->draw(textVictory);
        this->window->draw(buttonPlay);
        this->window->draw(textPlay);
        this->window->draw(buttonExit);
        this->window->draw(textExit);
        this->window->display();
    }
    
    // Actuar según la opción seleccionada
    if (opcionSeleccionada == 1) {
        // Reiniciar el juego sin cerrar la ventana principal.
        this->reiniciarJuego();
        musicaDerrota.stop();
        musicaVictoria.stop();
    }
    else if (opcionSeleccionada == 2) {
        // Cerrar la ventana principal del juego.
        this->window->close();
    }
}

void Cuatroenlinea::jugarContraUltron()
{
    const int profundidad = 5;
    int mejorColumna = -1;
    int mejorValor = -10000;

    // Función lambda para evaluar el estado actual del tablero.
    auto evaluarBoard = [this]() -> int {
        int score = 0;
        // Ventaja al ocupar la columna central.
        int centerCol = COLUMNAS / 2;
        int centerCount = 0;
        for (int i = 0; i < FILAS; i++) {
            if (this->tablero[i][centerCol] == 'X')
                centerCount++;
        }
        score += centerCount * 3;

        // Evaluación horizontal.
        for (int i = 0; i < FILAS; i++) {
            for (int j = 0; j < COLUMNAS - 3; j++) {
                int countX = 0, countO = 0, countEmpty = 0;
                for (int k = 0; k < 4; k++) {
                    char cell = this->tablero[i][j + k];
                    if (cell == 'X') countX++;
                    else if (cell == 'O') countO++;
                    else countEmpty++;
                }
                if (countX == 4) return 1000; // Victoria inmediata.
                if (countO == 4) return -1000; // Derrota inmediata.
                if (countX == 3 && countEmpty == 1) score += 5;
                else if (countX == 2 && countEmpty == 2) score += 2;
                if (countO == 3 && countEmpty == 1) score -= 4;
            }
        }

        // Evaluación vertical.
        for (int j = 0; j < COLUMNAS; j++) {
            for (int i = 0; i < FILAS - 3; i++) {
                int countX = 0, countO = 0, countEmpty = 0;
                for (int k = 0; k < 4; k++) {
                    char cell = this->tablero[i + k][j];
                    if (cell == 'X') countX++;
                    else if (cell == 'O') countO++;
                    else countEmpty++;
                }
                if (countX == 4) return 1000;
                if (countO == 4) return -1000;
                if (countX == 3 && countEmpty == 1) score += 5;
                else if (countX == 2 && countEmpty == 2) score += 2;
                if (countO == 3 && countEmpty == 1) score -= 4;
            }
        }

        // Evaluación diagonal (positiva).
        for (int i = 0; i < FILAS - 3; i++) {
            for (int j = 0; j < COLUMNAS - 3; j++) {
                int countX = 0, countO = 0, countEmpty = 0;
                for (int k = 0; k < 4; k++) {
                    char cell = this->tablero[i + k][j + k];
                    if (cell == 'X') countX++;
                    else if (cell == 'O') countO++;
                    else countEmpty++;
                }
                if (countX == 4) return 1000;
                if (countO == 4) return -1000;
                if (countX == 3 && countEmpty == 1) score += 5;
                else if (countX == 2 && countEmpty == 2) score += 2;
                if (countO == 3 && countEmpty == 1) score -= 4;
            }
        }
        return score;
    };

    // Implementación del algoritmo Minimax con poda alfa-beta.
    function<int(int, int, int, bool)> minimaxAI = [this, &evaluarBoard, &minimaxAI](int depth, int alpha, int beta, bool maximizing) -> int 
    {
        int score = evaluarBoard();
        if (score == 1000 || score == -1000 || depth == 0)
            return score;
        
        if (maximizing) { // Turno de Ultron (IA)
            int maxEval = -10000;
            for (int col = 0; col < COLUMNAS; col++) {
                if (this->tablero[0][col] == ' ') {
                    int filaDisponible = -1;
                    for (int i = FILAS - 1; i >= 0; i--) {
                        if (this->tablero[i][col] == ' ') {
                            filaDisponible = i;
                            break;
                        }
                    }
                    if (filaDisponible == -1) continue;
                    this->tablero[filaDisponible][col] = 'X';
                    int eval = minimaxAI(depth - 1, alpha, beta, false);
                    this->tablero[filaDisponible][col] = ' ';
                    maxEval = max(maxEval, eval);
                    alpha = max(alpha, eval);
                    if (beta <= alpha) break; // Poda alfa-beta.
                }
            }
            return maxEval;
        } else { // Turno del oponente.
            int minEval = 10000;
            for (int col = 0; col < COLUMNAS; col++) {
                if (this->tablero[0][col] == ' ') {
                    int filaDisponible = -1;
                    for (int i = FILAS - 1; i >= 0; i--) {
                        if (this->tablero[i][col] == ' ') {
                            filaDisponible = i;
                            break;
                        }
                    }
                    if (filaDisponible == -1) continue;
                    this->tablero[filaDisponible][col] = 'O';
                    int eval = minimaxAI(depth - 1, alpha, beta, true);
                    this->tablero[filaDisponible][col] = ' ';
                    minEval = min(minEval, eval);
                    beta = min(beta, eval);
                    if (beta <= alpha) break;
                }
            }
            return minEval;
        }
    };

    // Buscar la mejor jugada para Ultron.
    for (int col = 0; col < COLUMNAS; col++) {
        if (this->tablero[0][col] == ' ') {
            int filaDisponible = -1;
            for (int i = FILAS - 1; i >= 0; i--) {
                if (this->tablero[i][col] == ' ') {
                    filaDisponible = i;
                    break;
                }
            }
            if (filaDisponible == -1) continue;
            this->tablero[filaDisponible][col] = 'X';
            int score = minimaxAI(profundidad - 1, -10000, 10000, false);
            this->tablero[filaDisponible][col] = ' ';
            if (score > mejorValor) {
                mejorValor = score;
                mejorColumna = col;
            }
        }
    }
    
    if (mejorColumna != -1) {
        this->colocarFicha(mejorColumna);
        
        vector<pair<int, int>> fichasGanadoras;
        if (this->verificarVictoria('X', fichasGanadoras)) {
            this->parpadearFichasGanadoras(fichasGanadoras);
            // Si Ultron gana, se muestra la ventana de victoria.
            this->musicaDerrota.setLoop(true);
            this->musicaDerrota.play();
            this->mostrarVentanaVictoria("Victoria de Ultron");
        }
        else {
            this->cambiarTurno();
        }
    }
}

void Cuatroenlinea::parpadearFichasGanadoras(const vector<pair<int, int>>& fichasGanadoras) {
    
    const int numParpadeos = 5;
    const sf::Time duracionParpadeo = sf::milliseconds(500);

    for (int i = 0; i < numParpadeos; ++i) {
        for (const auto& ficha : fichasGanadoras) {
            int fila = ficha.first;
            int columna = ficha.second;

            sf::CircleShape fichaShape(40.0f);
            fichaShape.setPosition(10 + columna * (100.0f), (fila + 1) * (100.0f));
            fichaShape.setOutlineThickness(1.75f);
            fichaShape.setOutlineColor(sf::Color(96, 109, 166));
            fichaShape.setFillColor((i % 2 == 0) ? sf::Color::White : (tablero[fila][columna] == 'O' ? sf::Color(255, 105, 97) : sf::Color(255, 253, 150)));

            this->window->draw(fichaShape);
        }
        this->window->display();
        fadeOutMusica(this->musicaFondo, 2.0f);
        this->sonidoBip.play(); // Reproducir sonido en cada parpadeo
        sf::sleep(duracionParpadeo);
    }
}

void Cuatroenlinea::mostrarMenu()
{
    this->musicaFondo.setLoop(true);
    this->musicaFondo.play();
    sf::Font menuFont;
    if (!menuFont.loadFromFile("resources/font.ttf"))
    {
        cerr << "Error al cargar la fuente" << endl;
        return;
    }

    sf::Text titulo("4 en Linea", menuFont, 20);
    titulo.setFillColor(sf::Color::Black);
    titulo.setPosition(250, 100);

    sf::RectangleShape caja_contra_jugador(sf::Vector2f(400, 50));
    caja_contra_jugador.setFillColor(sf::Color(105, 97, 255));
    caja_contra_jugador.setOutlineColor(sf::Color::Black);
    caja_contra_jugador.setOutlineThickness(2);
    caja_contra_jugador.setPosition(150, 240);

    sf::Text opcion_contra_jugador("Jugar contra otro jugador", menuFont, 15);
    opcion_contra_jugador.setFillColor(sf::Color(242, 240, 235));
    opcion_contra_jugador.setPosition(162, 255);

    sf::RectangleShape caja_contra_ia(sf::Vector2f(400, 50));
    caja_contra_ia.setFillColor(sf::Color(105, 97, 255));
    caja_contra_ia.setOutlineColor(sf::Color::Black);
    caja_contra_ia.setOutlineThickness(2);
    caja_contra_ia.setPosition(150, 340);

    sf::Text opcion_contra_ia("Jugar contra la IA", menuFont, 15);
    opcion_contra_ia.setFillColor(sf::Color(242, 240, 235));
    opcion_contra_ia.setPosition(210, 355);

    while (this->window->isOpen())
    {
        sf::Event event;
        while (this->window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                fadeOutMusica(this->musicaFondo, 2.0f); // Fade out de 2 segundos
                this->window->close();
            }

            if (event.type == sf::Event::MouseMoved)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);

                if (caja_contra_jugador.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                {
                    if (!sonidoCaja1Reproducido)
                    {
                        sonidoBoton.play();
                        sonidoCaja1Reproducido = true;
                    }
                }
                else
                {
                    sonidoCaja1Reproducido = false;
                }

                if (caja_contra_ia.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                {
                    if (!sonidoCaja2Reproducido)
                    {
                        sonidoBoton.play();
                        sonidoCaja2Reproducido = true;
                    }
                }
                else
                {
                    sonidoCaja2Reproducido = false;
                }
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);

                    if (caja_contra_jugador.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                    {
                        sonidoInicio.play();
                        fadeOutMusica(this->musicaFondo, 2.0f); 
                        sleep(seconds(1));
                        this->musicaFondo.setLoop(true);
                        musicaFondo.setVolume(75);
                        musicaFondo.play();
                        this->contraIA = false;
                        return;
                    }
                    else if (caja_contra_ia.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                    {
                        this->contraIA = true;

                        sf::RectangleShape cajaFacil(sf::Vector2f(300, 50));
                        cajaFacil.setFillColor(sf::Color::Transparent);
                        cajaFacil.setOutlineColor(sf::Color::Black);
                        cajaFacil.setOutlineThickness(2);
                        cajaFacil.setPosition(200, 240);

                        sf::Text opcionFacil("Modo Aspiradora", menuFont, 15);
                        opcionFacil.setFillColor(sf::Color::Black);
                        opcionFacil.setPosition(240, 255);

                        sf::RectangleShape cajaDificil(sf::Vector2f(300, 50));
                        cajaDificil.setFillColor(sf::Color::Transparent);
                        cajaDificil.setOutlineColor(sf::Color::Black);
                        cajaDificil.setOutlineThickness(2);
                        cajaDificil.setPosition(200, 340);

                        sf::Text opcionDificil("Modo Ultron", menuFont, 15);
                        opcionDificil.setFillColor(sf::Color::Black);
                        opcionDificil.setPosition(270, 355);

                        while (this->window->isOpen())
                        {
                            sf::Event subEvent;
                            while (this->window->pollEvent(subEvent))
                            {
                                if (subEvent.type == sf::Event::Closed) {
                                    fadeOutMusica(this->musicaFondo, 2.0f); // Fade out de 2 segundos
                                    this->window->close();
                                }

                                if (subEvent.type == sf::Event::MouseMoved)
                                {
                                    sf::Vector2i subMousePos = sf::Mouse::getPosition(*this->window);

                                    if (cajaFacil.getGlobalBounds().contains(static_cast<sf::Vector2f>(subMousePos)))
                                    {
                                        if (!sonidoCaja1Reproducido)
                                        {
                                            sonidoBoton.play();
                                            sonidoCaja1Reproducido = true;
                                        }
                                    }
                                    else
                                    {
                                        sonidoCaja1Reproducido = false;
                                    }

                                    if (cajaDificil.getGlobalBounds().contains(static_cast<sf::Vector2f>(subMousePos)))
                                    {
                                        if (!sonidoCaja2Reproducido)
                                        {
                                            sonidoBoton.play();
                                            sonidoCaja2Reproducido = true;
                                        }
                                    }
                                    else
                                    {
                                        sonidoCaja2Reproducido = false;
                                    }
                                }

                                if (subEvent.type == sf::Event::MouseButtonPressed)
                                {
                                    if (subEvent.mouseButton.button == sf::Mouse::Left)
                                    {
                                        sf::Vector2i subMousePos = sf::Mouse::getPosition(*this->window);

                                        if (cajaFacil.getGlobalBounds().contains(static_cast<sf::Vector2f>(subMousePos)))
                                        {
                                            sonidoInicio.play();
                                            fadeOutMusica(this->musicaFondo, 1.5f); // Fade out de 2 segundos
                                            sleep(sf::seconds(1));
                                            this->dificultadIA = "Aspiradora";
                                            this->musicaFondo.setLoop(true);
                                            musicaFondo.setVolume(50);
                                            musicaFondo.play();
                                            return;
                                        }
                                        else if (cajaDificil.getGlobalBounds().contains(static_cast<sf::Vector2f>(subMousePos)))
                                        {
                                            sonidoInicio.play();
                                            fadeOutMusica(this->musicaFondo, 1.5f); // Fade out de 2 segundos
                                            sleep(sf::seconds(1));
                                            this->musicaFondo.setLoop(true);
                                            musicaFondo.setVolume(50);
                                            musicaFondo.play();
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

        // Verifica si el mouse está sobre las cajas y cambia su color
        sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);

        if (caja_contra_jugador.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
        {
            opcion_contra_jugador.setFillColor(sf::Color::Red);
        }
        else
        {
            opcion_contra_jugador.setFillColor(sf::Color(242, 240, 235));
        }

        if (caja_contra_ia.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
        {
            opcion_contra_ia.setFillColor(sf::Color::Red);
        }
        else
        {
            opcion_contra_ia.setFillColor(sf::Color(242, 240, 235));
        }

        this->window->clear(sf::Color::White);
        this->window->draw(titulo);
        this->window->draw(caja_contra_jugador);
        this->window->draw(opcion_contra_jugador);
        this->window->draw(caja_contra_ia);
        this->window->draw(opcion_contra_ia);
        this->window->display();
    }
}
