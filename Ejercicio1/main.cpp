#include "header.hpp"

int main(){

    //Punto b: Crear 3 pokemones e imprimirlos todos
    Pokemon sq("Squirtle", 100);
    unordered_map<string, int> ataques_sq = {{"Pistola Agua", 4}, {"Hidrobomba", 6}, {"Danza Lluvia", 5}};
    array<int,3> experiencia_sq = {0, 400, 1000};
    PokemonInfo info_sq("Agua", "Una tortuga pequeña que lanza chorros de agua.", ataques_sq, experiencia_sq);


    Pokedex pokedex("archivo.bin");
    pokedex.agregarPokemon(sq, info_sq);
    array<int,3> experiencia_bul = {0,300,1100};
    pokedex.agregarPokemon(Pokemon("Bulbasaur", 270), PokemonInfo("Planta", "Tiene una semilla en su lomo que crece con el tiempo", {{"Latigazo", 4}, {"Hoja Afilada", 6}, {"Rayo Solar", 5}}, experiencia_bul));

    array<int, 3> experiencia_ch = {0, 250, 1300};
    pokedex.agregarPokemon(Pokemon("Charmander", 633), PokemonInfo("Fuego", "Una lagartija con una llama en su cola", {{"Ascuas", 4}, {"Lanzallamas", 6}, {"Giro Fuego", 5}}, experiencia_ch));

    pokedex.mostrarTodos();

    //Punto c: metodo mostrar y buscar en el pokedex
    cout<<"Metodo Mostrar (individual de un Pokemon):\n";
    pokedex.mostrar(Pokemon("Squirtle", 870)); //Lo encuentra
    pokedex.mostrar(Pokemon("Pikachu", 390)); //No lo encuentra

    //Punto d: Se crea ptro pokedex vacio y se pasa el archivo generado por el pokedex original. 
    // Se imprimen todos los pokemones del nuevo pokedex
    cout<<"\nOPCIONAL\n";
    Pokedex pokedex2("archivo.bin");
    pokedex2.mostrarTodos();
}