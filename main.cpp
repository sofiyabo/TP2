#include "header.hpp"

int main(){
    Pokemon sq("Squirtle", 100);
    unordered_map<string, int> ataques_sq = {{"Pistola Agua", 4}, {"Hidrobomba", 6}, {"Danza Lluvia", 5}};
    array<int,3> experiencia_sq = {0, 400, 1000};
    PokemonInfo info_sq("Agua", "Una tortuga pequeña que lanza chorros de agua.", ataques_sq, experiencia_sq);


    Pokedex pokedex;
    pokedex.agregarPokemon(sq, info_sq);
    array<int,3> experiencia_bul = {0,300,1100};
    pokedex.agregarPokemon(Pokemon("Bulbasaur", 270), PokemonInfo("Planta", "Tiene una semilla en su lomo que crece con el tiempo", {{"Pistola Agua", 4}, {"Hidrobomba", 6}, {"Danza Lluvia", 5}}, experiencia_bul));
    pokedex.mostrarTodos();
}