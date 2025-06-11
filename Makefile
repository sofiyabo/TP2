all:
	g++ -std=c++17 -g -o ej1 source.cpp main.cpp
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./ej1