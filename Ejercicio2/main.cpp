#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include "2.hpp"

int main() {
    std::mutex consola;  // Mutex que protege el acceso al cout
    std::mutex sectores[5]; // array de mutex para las 5 zonas adyacentes
    std::thread procesos[5]; // array de threads para los drones

    for (int i = 0; i < 5; ++i) {
        int zonaA = i;         // asignamos las zonas adyacentes a cada dron
        int zonaB = (i + 4) % 5;    
        procesos[i] = std::thread(Dron(i, &sectores[zonaB], &sectores[zonaA], consola)); // Creamos un thread para cada dron, pasando los mutex de las zonas adyacentes y el mutex del cout
    }

    for (auto& t : procesos) { // Esperamos a que todos los threads terminen
        t.join();
    }

    return 0;  
}
