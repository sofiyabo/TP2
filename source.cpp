#include "header.hpp"


ostream& operator<<(ostream& os, const PokemonInfo& poke_info){
    os << "Tipo: " << poke_info.tipo << "\n"
    << "Descripcion: " << poke_info.descripcion << "\n"
    << "Ataques por nivel: \n";
    for (const auto& ataque : poke_info.ataquesDisponiblesPorNivel){
        os << ataque.first << ": " << ataque.second <<" de daño\n"; 
    }

    os << "Experiencia por nivel:\n";
    for(int i = 1; i<4; i++){
        os << "Nivel " << i <<": " << poke_info.experienciaProximoNivel[i-1]<< " de experiencia\n";

    }
    return os;
}

void Pokedex::agregarPokemon(const Pokemon& poke, const PokemonInfo& poke_info){
    pokedex[poke] = poke_info;
}

void Pokedex::mostrar(const Pokemon& poke) const{
    //find del pokemon en el unordered_map
    auto iterador = pokedex.find(poke);
    if(iterador != pokedex.end()){
        //imprimir PokemonInfo
        cout<< iterador->second;
    }
    else{
        cout<<"Pokemon desconocido!\n";
    }

}

void Pokedex::mostrarTodos() const {
    for (const auto& poke : pokedex) { // Itera sobre cada elemento en el mapa
        cout<< "Pokemon: " << poke.first.getNombre()<< "\n";
        mostrar(poke.first); // Llama a `mostrar()` pasando la clave (Pokemon)
        cout<<"\n";
    }
}