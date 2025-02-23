all: compile

compile:
	g++ -c src/main.cpp -I"src/SFML-3.0.0/include" -o src/main.o 
	g++ src/main.o -o src/main -L"src/SFML-3.0.0/lib" -lsfml-graphics -lsfml-window -lsfml-system


clean:
	if exist src\main.o del src\main.o
	if exist src\main del src\main
