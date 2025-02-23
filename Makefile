all: compile


build:
	if not exist build mkdir build

compile: build
	g++ -c src/main.cpp -I"src/SFML-3.0.0/include" -o build/main.o 
	g++ build/main.o -o build/main -L"src/SFML-3.0.0/lib" -lsfml-graphics -lsfml-window -lsfml-system

clean:
	# Eliminar los archivos generados en la carpeta build
	if exist build\main.o del build\main.o
	if exist build\main del build\main
