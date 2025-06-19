#pragma once

#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>

class Dron {
private:
    int identificador;   // identificador del dron
    std::mutex* zona1;  //punteros a mutex las zonas adyacentes
    std::mutex* zona2;
    std::mutex& mutexSalida; // referencia a mutex para los cout

public:
    Dron(int id, std::mutex* z1, std::mutex* z2, std::mutex& m);  // Constructor que inicializa el dron con su id y mutex de zonas
    void operator()();  // Sobrecarga del operador () para que el objeto Dron pueda ser ejecutado como funcion en un hilo
};
