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

void Pokedex::agregarEnArchivo() const {
    ofstream out(archivo_info, ios::binary);
    if (!out) {
        cerr << "Error al abrir el archivo para escritura." << endl;
        return;
    }

    for (const auto& [poke, info] : pokedex) {

        string nombre = poke.getNombre();
        size_t nombreSize = nombre.size();
        out.write(reinterpret_cast<const char*>(&nombreSize), sizeof(nombreSize));
        out.write(nombre.c_str(), nombreSize);

        int experiencia = poke.getExperiencia();
        out.write(reinterpret_cast<const char*>(&experiencia), sizeof(experiencia));

        string tipo = info.getTipo();
        size_t tipoSize = tipo.size();
        out.write(reinterpret_cast<const char*>(&tipoSize), sizeof(tipoSize));
        out.write(tipo.c_str(), tipoSize);

        string descripcion = info.getDescripcion();
        size_t descSize = descripcion.size();
        out.write(reinterpret_cast<const char*>(&descSize), sizeof(descSize));
        out.write(descripcion.c_str(), descSize);

        const auto& ataques = info.getAtaques();
        size_t cantidadAtaques = ataques.size();
        out.write(reinterpret_cast<const char*>(&cantidadAtaques), sizeof(cantidadAtaques));

        for (const auto& [nombreAtaque, nivel] : ataques) {
            size_t ataqueSize = nombreAtaque.size();
            out.write(reinterpret_cast<const char*>(&ataqueSize), sizeof(ataqueSize));
            out.write(nombreAtaque.c_str(), ataqueSize);
            out.write(reinterpret_cast<const char*>(&nivel), sizeof(nivel));
        }

        const auto& experienciaNiveles = info.getExperiencias(); // array<int,3>
        out.write(reinterpret_cast<const char*>(experienciaNiveles.data()), sizeof(int) * 3);
    }

    out.close(); // opcional, destructor lo hace
}

void Pokedex::leerDesdeArchivo() {
    ifstream in(archivo_info, ios::binary);
    if (!in) {
        cerr << "Error al abrir el archivo para lectura." << endl;
        return;
    }

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

            string nombreAtaque(ataqueSize, '\0');
            in.read(&nombreAtaque[0], ataqueSize);

            int nivel;
            in.read(reinterpret_cast<char*>(&nivel), sizeof(nivel));

            ataques[nombreAtaque] = nivel;
        }

        array<int, 3> experienciaNiveles;
        in.read(reinterpret_cast<char*>(experienciaNiveles.data()), sizeof(int) * 3);

        // Crear los objetos y ponerlos en el pokedex
        Pokemon poke(nombre, experienciaPoke);
        PokemonInfo info(tipo, descripcion, ataques, experienciaNiveles);

        pokedex[poke] = info;
    }

}


void Pokedex::agregarPokemon(const Pokemon& poke, const PokemonInfo& poke_info){
    pokedex[poke] = poke_info;
    agregarEnArchivo();
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