CXX = g++
CXXFLAGS = -std=c++17
LDFLAGS = 

# Configuración para Linux
CXXFLAGS += -I/usr/include/SFML
LDFLAGS += -lsfml-graphics -lsfml-window -lsfml-system
RM = rm -f
EXE =
RUN = ./build/main

SRC_DIR = src
BUILD_DIR = build
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

.PHONY: all clean run install

# El objetivo principal (default) compila y ejecuta el programa
all: install $(BUILD_DIR)/main run

# Instalar SFML (solo para Ubuntu)
install:
	@echo "Instalando SFML..."
	sudo apt-get install -y libsfml-dev

# Compilar y enlazar el ejecutable
$(BUILD_DIR)/main: $(OBJ_FILES)
	@echo "Enlazando el ejecutable..."
	$(CXX) $(OBJ_FILES) -o $(BUILD_DIR)/main$(EXE) $(LDFLAGS)

# Compilar los archivos fuente
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compilando $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ejecutar el programa automáticamente tras compilar
run:
	@echo "Ejecutando el programa..."
	@if [ -f $(BUILD_DIR)/main$(EXE) ]; then \
		$(RUN); \
	else \
		echo "Error: El ejecutable no fue encontrado."; \
	fi

# Limpiar archivos generados
clean:
	@echo "Limpiando archivos generados..."
	$(RM) $(BUILD_DIR)/*.o $(BUILD_DIR)/main$(EXE)