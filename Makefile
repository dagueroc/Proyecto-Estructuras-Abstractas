CXX = g++
CXXFLAGS = -std=c++17
LDFLAGS = 

# Configuración para Linux
CXXFLAGS += -I/usr/include/SFML
LDFLAGS += -lsfml-graphics -lsfml-window -lsfml-system
RM = rm -f
EXE =
RUN = ./build/main

.PHONY: all clean run install

# El objetivo principal (default) compila y ejecuta el programa
all: install build/main run

# Instalar SFML (solo para Ubuntu)
install:
	@echo "Instalando SFML..."
	sudo apt-get install -y libsfml-dev

# Compilar y enlazar el ejecutable
build/main: build/main.o
	@echo "Enlazando el ejecutable..."
	$(CXX) build/main.o -o build/main$(EXE) $(LDFLAGS)

# Compilar el archivo fuente
build/main.o: src/main.cpp
	@echo "Compilando src/main.cpp..."
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o build/main.o

# Ejecutar el programa automáticamente tras compilar
run:
	@echo "Ejecutando el programa..."
	@if [ -f build/main$(EXE) ]; then \
		$(RUN); \
	else \
		echo "Error: El ejecutable no fue encontrado."; \
	fi

# Limpiar archivos generados
clean:
	@echo "Limpiando archivos generados..."
	$(RM) build/*.o build/main$(EXE)