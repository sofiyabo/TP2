#pragma once

#include <iostream>
#include <unordered_map>
#include <array>
#include <fstream>

using namespace std;
/*
Consultas:
    - El pokemon que se busca puede bbuscarse con experiencia diferente a la guardada?
*/

//Cada tipo de Pokemon tiene una información específica y única.

class Pokemon{ // del que se quiere la info
    private:
    string nombre;
    int experiencia; //tiene que ser >0 

    public:
    Pokemon(string nomb, int exp) : nombre(nomb), experiencia(exp) {} //completar constructor
    /*
    void setNombre(string nomb) {nombre = nomb;}
    void setExperiencia(int exp);
    */
    string getNombre() const {return nombre;}
    int getExperiencia() const {return experiencia;}
    
    //sobrecargar operador == para compararlos como claves, osea para search
    bool operator==(const Pokemon& poke) const{
        return nombre == poke.nombre;
    }

};

class PokemonInfo{ // posee la info (composicion?)
    private:
    string tipo;
    string descripcion; 
    unordered_map<string, int> ataquesDisponiblesPorNivel; //busqueda eficiente en o1 en gral y no necesito orden, aparte no permite repetidos
    array<int,3> experienciaProximoNivel; // uso array porque hay un numero fijo de elementos
    
    public:
    PokemonInfo(string t, string descr, unordered_map<string, int> ataques, array<int,3>& experiencia) : tipo(t), descripcion(descr), ataquesDisponiblesPorNivel(ataques), experienciaProximoNivel(experiencia) {}
    PokemonInfo(){}

    string getTipo() const { return tipo;}
    string getDescripcion()const {return descripcion;} 
    unordered_map<string, int> getAtaques() const {return ataquesDisponiblesPorNivel;}
    array<int, 3> getExperiencias() const {return experienciaProximoNivel;}
    friend ostream& operator<<(ostream& os, const PokemonInfo& info); //sobrecarga usando friend


};

class PokemonHash{
    public:
    size_t operator()(const Pokemon& poke) const {
        return hash<string>()(poke.getNombre());
    }
};

class Pokedex{ // contenedor principal para la info --> Composicion
    private:
    unordered_map<Pokemon, PokemonInfo, PokemonHash> pokedex; //se especifica pokemonhash como metodo de hashing para el map
    string archivo_info;
    //no permite claves repetidas!!! osea hay un pokemon por nombre

    //void cargaInicialArchivo();
    void agregarEnArchivo() const;

    public:
    //implementar metodos para que Pokemon pueda ser clave
    Pokedex(){}
    Pokedex(string archivo): archivo_info(archivo) { leerDesdeArchivo();}

    void leerDesdeArchivo();
    void agregarPokemon(const Pokemon& poke, const PokemonInfo& poke_info);
    void mostrar(const Pokemon& poke) const; //funciona como un search + imprimir
    void mostrarTodos() const;
    //void cargarDesdeArchivo();
};

