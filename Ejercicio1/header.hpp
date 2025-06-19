#pragma once

#include <iostream>
#include <unordered_map>
#include <array>
#include <fstream>

using namespace std;


class Pokemon{
    private:
    string nombre;
    int experiencia;  

    public:
    Pokemon(string nomb, int exp) : nombre(nomb), experiencia(exp) {
        if(exp<= 0){
            throw invalid_argument("La experiencia debe ser un entero positivo.");
        }
    } 
    string getNombre() const {return nombre;}
    int getExperiencia() const {return experiencia;}
    
    //sobrecargar operador == para compararlos como claves, para poder buscar en el pokedex.
    bool operator==(const Pokemon& poke) const{
        return nombre == poke.nombre;
    }

};

class PokemonInfo{ 
    private:
    string tipo;
    string descripcion; 
    unordered_map<string, int> ataquesDisponiblesPorNivel; 
    array<int,3> experienciaProximoNivel; 
    
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

class Pokedex{ 
    private:
    unordered_map<Pokemon, PokemonInfo, PokemonHash> pokedex; //Se especifica pokemonhash como metodo de hashing para el map
    string archivo_info;

    //Los dos metodos de modificar el archivo son privados para evitar que se usen directamente.
    void agregarEnArchivo(const Pokemon& poke, const PokemonInfo& poke_info) const;
    void leerDesdeArchivo();

    public:
    Pokedex(string archivo): archivo_info(archivo) { leerDesdeArchivo();}


    void agregarPokemon(const Pokemon& poke, const PokemonInfo& poke_info);
    void mostrar(const Pokemon& poke) const; //Funciona como un search + imprimir
    void mostrarTodos() const;
};

