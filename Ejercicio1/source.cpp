#include "header.hpp"

ostream& operator<<(ostream& os, const PokemonInfo& poke_info){
    /*
    Sobrecarga de operador<< para imprimir la informacion de un objeto PokemonInfo.
    */
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


void Pokedex::agregarEnArchivo(const Pokemon& poke, const PokemonInfo& poke_info) const {
    /*
    Recibe un Pokemon y su PokemonInfo, y agrega toda la informacion al archivo binario.
    */
    ofstream out(archivo_info, ios::binary | ios::app); 
    if (!out) {
        cerr << "Error al abrir el archivo." << endl;
        return;
    }

    string nombre = poke.getNombre();
    size_t nombreSize = nombre.size();
    out.write(reinterpret_cast<const char*>(&nombreSize), sizeof(nombreSize));
    out.write(nombre.c_str(), nombreSize);

    int experiencia = poke.getExperiencia();
    out.write(reinterpret_cast<const char*>(&experiencia), sizeof(experiencia));

    string tipo = poke_info.getTipo();
    size_t tipoSize = tipo.size();
    out.write(reinterpret_cast<const char*>(&tipoSize), sizeof(tipoSize));
    out.write(tipo.c_str(), tipoSize);

    string descripcion = poke_info.getDescripcion();
    size_t descSize = descripcion.size();
    out.write(reinterpret_cast<const char*>(&descSize), sizeof(descSize));
    out.write(descripcion.c_str(), descSize);

    unordered_map<string, int> ataques = poke_info.getAtaques();
    size_t cantidadAtaques = ataques.size();
    out.write(reinterpret_cast<const char*>(&cantidadAtaques), sizeof(cantidadAtaques));

    for (const auto& [ataque, potencia] : ataques) {
        size_t ataqueSize = ataque.size();
        out.write(reinterpret_cast<const char*>(&ataqueSize), sizeof(ataqueSize));
        out.write(ataque.c_str(), ataqueSize);
        out.write(reinterpret_cast<const char*>(&potencia), sizeof(potencia));
        }

    array<int, 3> experienciaNiveles = poke_info.getExperiencias(); 
    out.write(reinterpret_cast<const char*>(experienciaNiveles.data()), sizeof(int) * 3);
    
}



void Pokedex::leerDesdeArchivo() {
    /*
    Al crear un Pokedex con un archivo, lee el archivo binario y deserializa la informacion para crear
    los objetos Pokemon y PokemonInfo, que luego se guardan en el pokedex.
    */
    ifstream in(archivo_info, ios::binary);
    if (!in) {
        cerr << "Error al abrir el archivo para lectura." << endl;
        return;
    }

    //Lee el archivo hasta que termina. Para los strings y para el array, primero toma el tamaño y despues le pone el contenido.
    while (true) {
        size_t nombreSize;
        in.read(reinterpret_cast<char*>(&nombreSize), sizeof(nombreSize));
        if(in.eof()) break; //Si se llega al final del archivo, termina el loop

        string nombre(nombreSize, '\0');
        in.read(&nombre[0], nombreSize);

        int experienciaPoke;
        in.read(reinterpret_cast<char*>(&experienciaPoke), sizeof(experienciaPoke));

        size_t tipoSize;
        in.read(reinterpret_cast<char*>(&tipoSize), sizeof(tipoSize));
        string tipo(tipoSize, '\0');
        in.read(&tipo[0], tipoSize);

        size_t descSize;
        in.read(reinterpret_cast<char*>(&descSize), sizeof(descSize));
        string descripcion(descSize, '\0');
        in.read(&descripcion[0], descSize);

        size_t cantidadAtaques;
        in.read(reinterpret_cast<char*>(&cantidadAtaques), sizeof(cantidadAtaques));

        unordered_map<string, int> ataques;
        for (size_t i = 0; i < cantidadAtaques; i++) {
            size_t ataqueSize;
            in.read(reinterpret_cast<char*>(&ataqueSize), sizeof(ataqueSize));

            string ataque(ataqueSize, '\0');
            in.read(&ataque[0], ataqueSize);

            int potencia;
            in.read(reinterpret_cast<char*>(&potencia), sizeof(potencia));

            ataques[ataque] = potencia;
        }

        array<int, 3> experienciaNiveles;
        in.read(reinterpret_cast<char*>(experienciaNiveles.data()), sizeof(int) * 3);

        // Crear los objetos y ponerlos en el pokedex
        Pokemon poke(nombre, experienciaPoke);
        PokemonInfo info(tipo, descripcion, ataques, experienciaNiveles);

        pokedex[poke] = info; //No usa agregarpokemon para no volver a escribir en el archivo.
    }

}


void Pokedex::agregarPokemon(const Pokemon& poke, const PokemonInfo& poke_info){
    /*
    Agrega un un Pokemon y su PokemonInfo al pokedex. Tambien llama a la funcion agregarEnArchivo.
    */
    pokedex[poke] = poke_info;
    agregarEnArchivo(poke, poke_info);
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
    /*
    Imprime por pantalla todos los pokemones del pokedex. Usa el metodo mostrar para cada Pokemon.
    */
    for (const auto& poke : pokedex) { // Itera sobre cada elemento del pokedex
        cout<< "Pokemon: " << poke.first.getNombre()<< "\n";
        mostrar(poke.first); // Usa mostrar sobre el pokemon
        cout<<"\n";
    }
}