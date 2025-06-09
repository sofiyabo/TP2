#pragma once

#include <iostream>
#include <unordered_map>

using namespace std;

class Pokemon{ // del que se quiere la info
    private:
    string nombre;
    int experiencia; //tiene que ser >0 

    public:
    Pokemon() {} //completar constructor
    void setNombre(string nomb) {nombre = nomb;}
    void setExperiencia(int exp);
    string getNombre(){return nombre;}
    int getExperiencia(){return experiencia;}

};

class PokemonInfo{ // posee la info (composicion?)
    private:
    string tipo;
    string descripcion; 
    // ataquesDisponiblesPorNivel: contenedor, nombre ataque y puntaje de dano (int)
    // experienciaProximoNivel: cuanta experiencia se requiere para cada nivel
    
    public:
    
};

class Pokedex{ // contenedor principal para la info (??
    private:
    unordered_map<Pokemon, Pokedex> info;
    //implementar metodo para que Pokemon pueda ser clave
    // functor para hash usando solo el nombre del pokemon
    void mostrar(Pokemon pok);
    void mostrarTodos();
};